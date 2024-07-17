#include "Group/GroupRepository.hpp"

#include <oatpp/base/Log.hpp>

oatpp::Object<GroupDTO> GroupRepository::addGroup(oatpp::Object<GroupDTO> const& group)
{
    auto operation = dao_m.getObject()->addGroup(group);
    auto groupId = oatpp::sqlite::Utils::getLastInsertRowId(operation->getConnection());
    return getGroupById(static_cast<v_int32>(groupId));
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

