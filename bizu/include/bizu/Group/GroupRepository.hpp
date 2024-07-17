#pragma once

#include "Generic/DTO/StatusDTO.hpp"

#include "Group/Entities/Activity/ActivityRepository.hpp"
#include "Group/Entities/Member/MemberRepository.hpp"
#include "DTO/GroupInfoDTO.hpp"
#include "DTO/GroupDTO.hpp"
#include "DAO/GroupDAO.hpp"

#include <oatpp/Types.hpp>
#include <oatpp/web/protocol/http/Http.hpp>
#include <oatpp/macro/component.hpp>

class GroupRepository
{
private:
    OATPP_CREATE_COMPONENT(std::shared_ptr<GroupDAO>, dao_m)([] {
        OATPP_COMPONENT(std::shared_ptr<oatpp::provider::Provider<oatpp::sqlite::Connection>>, dbConnectionProvider);
        auto executor = std::make_shared<oatpp::sqlite::Executor>(dbConnectionProvider);
        return std::make_shared<GroupDAO>(executor);
    }());

    OATPP_CREATE_COMPONENT(std::shared_ptr<ActivityRepository>, activityRepository_m)([] {
        return std::make_shared<ActivityRepository>();
    }());

    OATPP_CREATE_COMPONENT(std::shared_ptr<MemberRepository>, memberRepository_m)([] {
        return std::make_shared<MemberRepository>();
    }());

public:
    oatpp::Object<GroupDTO> addGroup(oatpp::Object<GroupDTO> const& group);
    oatpp::Object<StatusDTO> removeGroupById(oatpp::Int64 const& groupId);
    oatpp::Object<GroupDTO> getGroupById(oatpp::Int64 const& groupId);
    oatpp::Vector<oatpp::Object<GroupDTO>> getGroups(oatpp::Int64 const& memberId);
    oatpp::Object<GroupInfoDTO> getInfo(oatpp::Int64 const& groupId);
};

