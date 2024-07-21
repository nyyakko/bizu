#include "Group/GroupRepository.hpp"

#include <oatpp/base/Log.hpp>
#include <fplus/fplus.hpp>
#include <fmt/format.h>

oatpp::Object<GroupDTO> GroupRepository::addGroup(oatpp::Object<GroupDTO> const& group)
{
    auto operation = dao_m.getObject()->addGroup(group);
    auto groupId = oatpp::sqlite::Utils::getLastInsertRowId(operation->getConnection());
    return getGroupById(groupId);
}

oatpp::Object<StatusDTO> GroupRepository::updateGroup(oatpp::Int64 const& groupId, oatpp::Object<GroupDTO> const& group)
{
    if (getGroupById(groupId) == nullptr) return nullptr; // FIXME: nulls are scary

    if (group->name) dao_m.getObject()->updateGroupName(groupId, group->name);

    auto status = StatusDTO::createShared();

    status->status = "OK";
    status->code = 200;
    return status;
}

oatpp::Object<StatusDTO> GroupRepository::removeGroupById(oatpp::Int64 const& groupId)
{
    auto operation = dao_m.getObject()->removeGroupById(groupId);
    auto status = StatusDTO::createShared();
    status->status = "OK";
    status->code = 200;
    return status;
}

oatpp::Object<GroupDTO> GroupRepository::getGroupById(oatpp::Int64 const& groupId)
{
    auto operation = dao_m.getObject()->getGroupById(groupId);
    if (!operation->hasMoreToFetch()) return nullptr; // FIXME: nulls are scary
    auto result = operation->fetch<oatpp::Vector<oatpp::Object<GroupDTO>>>();
    return result->at(0);
}

oatpp::Object<GroupDTO> GroupRepository::getGroupByInvite(oatpp::String const& invite)
{
    auto operation = dao_m.getObject()->getGroupByInvite(invite);
    if (!operation->hasMoreToFetch()) return nullptr; // FIXME: nulls are scary
    auto result = operation->fetch<oatpp::Vector<oatpp::Object<GroupDTO>>>();
    return result->at(0);
}

oatpp::Vector<oatpp::Object<GroupDTO>> GroupRepository::getGroups(oatpp::Int64 const& memberId)
{
    auto operation = dao_m.getObject()->getGroups(memberId);
    return operation->fetch<oatpp::Vector<oatpp::Object<GroupDTO>>>();
}

oatpp::Object<GroupInfoDTO> GroupRepository::getInfo(oatpp::Int64 const& groupId)
{
    auto information = GroupInfoDTO::createShared();
    information->id = groupId;
    information->name = dao_m.getObject()->getName(groupId)->fetch<oatpp::Vector<oatpp::Object<GroupInfoDTO>>>()->at(0)->name;
    information->members = memberRepository_m.getObject()->getMembers(groupId);
    information->activities = activityRepository_m.getObject()->getActivities(groupId);
    return information;
}

oatpp::Object<GroupInviteDTO> GroupRepository::createGroupInvite(oatpp::Int64 const& groupId)
{
    // NOTE: this is funny
    auto const invite { fplus::join(std::string{},
        fplus::transform([] (auto&& value) { return fmt::format("{}", value); }, fplus::generate<std::vector<char>>([] () {
            static std::random_device device {};
            static std::mt19937 generator { device() };
            static std::uniform_int_distribution<char> distribution { 'a', 'z' };
            return distribution(generator);
        }, 8)
    )) };

    auto operation = dao_m.getObject()->createGroupInvite(groupId, invite);
    auto result = GroupInviteDTO::createShared();
    result->value = invite;
    return result;
}
