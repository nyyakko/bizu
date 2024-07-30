#pragma once

#include "Generic/DTO/OperationDTO.hpp"

#include "MemberRepository.hpp"
#include "DTO/MemberDTO.hpp"

#include <oatpp/web/protocol/http/Http.hpp>
#include <oatpp/macro/component.hpp>

class MemberService
{
private:
    OATPP_COMPONENT(std::shared_ptr<MemberRepository>, repository_m);

public:
    Operation<oatpp::Object<MemberDTO>> addMember(oatpp::Int64 const& groupId, oatpp::Object<MemberDTO> const& member) { return repository_m->addMember(groupId, member); }
    Operation<void> removeMemberById(oatpp::Int64 const& groupId, oatpp::Int64 const& memberId) { return repository_m->removeMemberById(groupId, memberId); }
    Operation<oatpp::Vector<oatpp::Object<MemberDTO>>> getMembers(oatpp::Int64 const& groupId) { return repository_m->getMembers(groupId); }
    Operation<oatpp::Object<MemberDTO>> getMemberById(oatpp::Int64 const& groupId, oatpp::Int64 const& memberId) { return repository_m->getMemberById(groupId, memberId); }
};
