#pragma once

#include "GroupRepository.hpp"

#include "Generic/DTO/StatusDTO.hpp"
#include "DTO/GroupInfoDTO.hpp"
#include "DTO/GroupInviteDTO.hpp"
#include "DTO/GroupDTO.hpp"

#include <oatpp/web/protocol/http/Http.hpp>
#include <oatpp/macro/component.hpp>

class GroupService
{
private:
    OATPP_COMPONENT(std::shared_ptr<GroupRepository>, repository_m);

public:
    oatpp::Object<GroupDTO> addGroup(oatpp::Object<GroupDTO> const& group) { return repository_m->addGroup(group); }
    oatpp::Object<StatusDTO> updateGroup(oatpp::Int64 const& groupId, oatpp::Object<GroupDTO> const& group) { return repository_m->updateGroup(groupId, group); }
    oatpp::Object<StatusDTO> removeGroupById(oatpp::Int64 const& groupId) { return repository_m->removeGroupById(groupId); }
    oatpp::Object<GroupDTO> getGroupById(oatpp::Int64 const& groupId) { return repository_m->getGroupById(groupId); }
    oatpp::Object<GroupDTO> getGroupByInvite(oatpp::String const& invite) { return repository_m->getGroupByInvite(invite); }
    oatpp::Vector<oatpp::Object<GroupDTO>> getGroups(oatpp::Int64 const& memberId) { return repository_m->getGroups(memberId); }
    oatpp::Object<GroupInfoDTO> getInfo(oatpp::Int64 const& groupId) { return repository_m->getInfo(groupId); }
    oatpp::Object<GroupInviteDTO> createGroupInvite(oatpp::Int64 const& groupId) { return repository_m->createGroupInvite(groupId); }
};
