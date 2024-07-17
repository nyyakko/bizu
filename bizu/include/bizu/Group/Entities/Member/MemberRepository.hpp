#pragma once

#include "Generic/DTO/StatusDTO.hpp"

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
    oatpp::Object<MemberDTO> addMember(oatpp::Int64 const& groupId, oatpp::Object<MemberDTO> const& member);
    oatpp::Object<StatusDTO> removeMemberById(oatpp::Int64 const& groupId, oatpp::Int64 const& memberId);
    oatpp::Vector<oatpp::Object<MemberDTO>> getMembers(oatpp::Int64 const& groupId);
    oatpp::Object<MemberDTO> getMemberById(oatpp::Int64 const& groupId, oatpp::Int64 const& memberId);
};

