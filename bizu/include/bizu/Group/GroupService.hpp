#pragma once

#include "Generic/DTO/OperationDTO.hpp"
#include "GroupRepository.hpp"

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
    Operation<oatpp::Object<GroupDTO>> addGroup(oatpp::Object<GroupDTO> const& group) { return repository_m->addGroup(group); }
    Operation<void> updateGroup(oatpp::Int64 const& groupId, oatpp::Object<GroupDTO> const& group) { return repository_m->updateGroup(groupId, group); }
    Operation<void> removeGroupById(oatpp::Int64 const& groupId) { return repository_m->removeGroupById(groupId); }
    Operation<oatpp::Object<GroupDTO>> getGroupById(oatpp::Int64 const& groupId) { return repository_m->getGroupById(groupId); }
    Operation<oatpp::Object<GroupDTO>> getGroupByInvite(oatpp::String const& invite) { return repository_m->getGroupByInvite(invite); }
    Operation<oatpp::Vector<oatpp::Object<GroupDTO>>> getGroups(oatpp::Int64 const& memberId) { return repository_m->getGroups(memberId); }
    Operation<oatpp::Object<GroupInfoDTO>> getGroupInfo(oatpp::Int64 const& groupId) { return repository_m->getGroupInfo(groupId); }
    Operation<oatpp::Object<GroupInviteDTO>> createGroupInvite(oatpp::Int64 const& groupId) { return repository_m->createGroupInvite(groupId); }
};
