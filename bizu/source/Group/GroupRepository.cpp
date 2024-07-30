#include "Group/GroupRepository.hpp"

#include <oatpp/base/Log.hpp>
#include <fplus/fplus.hpp>
#include <fmt/format.h>

Operation<oatpp::Object<GroupDTO>> GroupRepository::addGroup(oatpp::Object<GroupDTO> const& group)
{
    auto status = Operation<oatpp::Object<GroupDTO>>::createShared();

    auto const operation = dao_m.getObject()->addGroup(group);
    if (operation->isSuccess())
    {
        status->value = getGroupById(oatpp::sqlite::Utils::getLastInsertRowId(operation->getConnection()))->value;
        status->status = "OK";
        status->code = 200;
    }
    else
    {
        status->status = fmt::format("SQL OPERATION FAILURE: {}", *operation->getErrorMessage());
        status->code = 500;
    }

    return status;
}

Operation<void> GroupRepository::updateGroup(oatpp::Int64 const& groupId, oatpp::Object<GroupDTO> const& group)
{
    auto status = Operation<void>::createShared();

    if (getGroupById(groupId)->code == 200)
    {
        if (group->name)
        {
            auto const operation = dao_m.getObject()->updateGroupName(groupId, group->name);
            if (!operation->isSuccess())
            {
                status->status = fmt::format("SQL OPERATION FAILURE: {}", *operation->getErrorMessage());
                status->code = 500;
                return status;
            }
        }

        status->status = "OK";
        status->code = 200;
    }
    else
    {
        status->status = "GROUP NOT FOUND";
        status->code = 404;
    }


    return status;
}

Operation<void> GroupRepository::removeGroupById(oatpp::Int64 const& groupId)
{
    auto status = Operation<void>::createShared();

    if (getGroupById(groupId)->code == 200)
    {
        auto const operation = dao_m.getObject()->removeGroupById(groupId);
        if (operation->isSuccess())
        {
            status->status = "OK";
            status->code = 200;
        }
        else
        {
            status->status = fmt::format("SQL OPERATION FAILURE: {}", *operation->getErrorMessage());
            status->code = 500;
        }
    }
    else
    {
        status->status = "GROUP NOT FOUND";
        status->code = 404;
    }


    return status;
}

Operation<oatpp::Object<GroupDTO>> GroupRepository::getGroupById(oatpp::Int64 const& groupId)
{
    auto status = Operation<oatpp::Object<GroupDTO>>::createShared();

    auto const operation = dao_m.getObject()->getGroupById(groupId);
    if (operation->isSuccess())
    {
        if (operation->hasMoreToFetch())
        {
            status->value = operation->fetch<oatpp::Vector<oatpp::Object<GroupDTO>>>()->at(0);
            status->status = "OK";
            status->code = 200;
        }
        else
        {
            status->status = "GROUP NOT FOUND";
            status->code = 404;
        }
    }
    else
    {
        status->status = fmt::format("SQL OPERATION FAILURE: {}", *operation->getErrorMessage());
        status->code = 500;
    }

    return status;
}

Operation<oatpp::Object<GroupDTO>> GroupRepository::getGroupByInvite(oatpp::String const& invite)
{
    auto status = Operation<oatpp::Object<GroupDTO>>::createShared();

    auto const operation = dao_m.getObject()->getGroupByInvite(invite);
    if (operation->isSuccess())
    {
        if (operation->hasMoreToFetch())
        {
            status->value = operation->fetch<oatpp::Vector<oatpp::Object<GroupDTO>>>()->at(0);
            status->status = "OK";
            status->code = 200;
        }
        else
        {
            status->status = "INVITE NOT FOUND";
            status->code = 404;
        }
    }
    else
    {
        status->status = fmt::format("SQL OPERATION FAILURE: {}", *operation->getErrorMessage());
        status->code = 500;
    }

    return status;
}

Operation<oatpp::Vector<oatpp::Object<GroupDTO>>> GroupRepository::getGroups(oatpp::Int64 const& memberId)
{
    auto status = Operation<oatpp::Vector<oatpp::Object<GroupDTO>>>::createShared();

    auto const operation = dao_m.getObject()->getGroups(memberId);
    if (operation->isSuccess())
    {
        status->value = operation->fetch<oatpp::Vector<oatpp::Object<GroupDTO>>>();
        status->status = "OK";
        status->code = 200;
    }
    else
    {
        status->status = fmt::format("SQL OPERATION FAILURE: {}", *operation->getErrorMessage());
        status->code = 500;
    }

    return status;
}

Operation<oatpp::Object<GroupInfoDTO>> GroupRepository::getGroupInfo(oatpp::Int64 const& groupId)
{
    auto status = Operation<oatpp::Object<GroupInfoDTO>>::createShared();

    auto result = GroupInfoDTO::createShared();
    result->id = groupId;

    if (auto operation = dao_m.getObject()->getName(groupId); operation->isSuccess())
    {
        result->name = dao_m.getObject()->getName(groupId)->fetch<oatpp::Vector<oatpp::Object<GroupInfoDTO>>>()->at(0)->name;
    }
    else
    {
        status->status = fmt::format("SQL OPERATION FAILURE: {}", *operation->getErrorMessage());
        status->code = 500;
        return status;
    }

    if (auto operation = memberRepository_m.getObject()->getMembers(groupId); operation->code == 200)
    {
        result->members = operation->value;
    }
    else
    {
        status->status = operation->status;
        status->code = operation->code;
        return status;
    }

    if (auto operation = activityRepository_m.getObject()->getActivities(groupId); operation->code == 200)
    {
        result->activities = operation->value;
    }
    else
    {
        status->status = operation->status;
        status->code = operation->code;
        return status;
    }

    status->value = result;
    status->status = "OK";
    status->code = 200;

    return status;
}

Operation<oatpp::Object<GroupInviteDTO>> GroupRepository::createGroupInvite(oatpp::Int64 const& groupId)
{
    auto status = Operation<oatpp::Object<GroupInviteDTO>>::createShared();

    // NOTE: this is funny
    auto const invite { fplus::join(std::string{},
        fplus::transform([] (auto&& value) { return fmt::format("{}", value); }, fplus::generate<std::vector<char>>([] () {
            static std::random_device device {};
            static std::mt19937 generator { device() };
            static std::uniform_int_distribution<char> distribution { 'a', 'z' };
            return distribution(generator);
        }, 8)
    )) };

    auto const operation = dao_m.getObject()->createGroupInvite(groupId, invite);
    if (operation->isSuccess())
    {
        auto result = GroupInviteDTO::createShared();
        result->value = invite;
        status->value = result;
        status->status = "OK";
        status->code = 200;
    }
    else
    {
        status->status = fmt::format("SQL OPERATION FAILURE: {}", *operation->getErrorMessage());;
        status->code = 200;
    }

    return status;
}
