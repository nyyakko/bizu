#pragma once

#include "Generic/DTO/OperationDTO.hpp"

#include "DTO/MemberDTO.hpp"
#include "DAO/MemberDAO.hpp"

#include <oatpp/Types.hpp>
#include <oatpp/web/protocol/http/Http.hpp>
#include <oatpp/macro/component.hpp>

class MemberRepository
{
private:
    OATPP_CREATE_COMPONENT(std::shared_ptr<MemberDAO>, dao_m)([] {
        OATPP_COMPONENT(std::shared_ptr<oatpp::provider::Provider<oatpp::sqlite::Connection>>, dbConnectionProvider);
        auto executor = std::make_shared<oatpp::sqlite::Executor>(dbConnectionProvider);
        return std::make_shared<MemberDAO>(executor);
    }());

public:
    Operation<oatpp::Object<MemberDTO>> addMember(oatpp::Int64 const& groupId, oatpp::Object<MemberDTO> const& member);
    Operation<void> removeMemberById(oatpp::Int64 const& groupId, oatpp::Int64 const& memberId);
    Operation<oatpp::Vector<oatpp::Object<MemberDTO>>> getMembers(oatpp::Int64 const& groupId);
    Operation<oatpp::Object<MemberDTO>> getMemberById(oatpp::Int64 const& groupId, oatpp::Int64 const& memberId);
};

