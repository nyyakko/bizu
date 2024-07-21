#pragma once

#include "Group/Entities/Member/DTO/MemberDTO.hpp"
#include "Group/GroupService.hpp"

#include "User/UserService.hpp"
#include "Group/Entities/Member/MemberService.hpp"
#include "Group/Entities/Activity/ActivityService.hpp"

#include <oatpp/web/server/api/ApiController.hpp>
#include <oatpp/macro/codegen.hpp>
#include <oatpp/macro/component.hpp>

#include <oatpp/base/Log.hpp>

using namespace oatpp::web::server;
using namespace oatpp::network;

#include OATPP_CODEGEN_BEGIN(ApiController)

class GroupController : public api::ApiController
{
private:
    GroupService groups_m;
    UserService users_m;
    ActivityService activities_m;
    MemberService members_m;

public:
    GroupController(OATPP_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, objectMapper))
        : api::ApiController(objectMapper)
    {
        setDefaultAuthorizationHandler(std::make_shared<handler::BearerAuthorizationHandler>(""));
    }

    ENDPOINT("GET", "groups/", getGroup, AUTHORIZATION(std::shared_ptr<handler::DefaultBearerAuthorizationObject>, auth))
    {
        auto requestee = users_m.getUserByAuth(auth->token);
        OATPP_ASSERT_HTTP(requestee != nullptr, Status::CODE_401, "Unauthorized");
        return createDtoResponse(Status::CODE_200, groups_m.getGroups(requestee->id));
    }

    ENDPOINT("POST", "groups/", addGroup, BODY_DTO(Object<GroupDTO>, group), AUTHORIZATION(std::shared_ptr<handler::DefaultBearerAuthorizationObject>, auth))
    {
        auto requestee = users_m.getUserByAuth(auth->token);
        OATPP_ASSERT_HTTP(requestee != nullptr, Status::CODE_401, "Unauthorized");

        auto owner = MemberDTO::createShared();
        owner->name = requestee->name;
        owner->id = requestee->id;
        owner->userRole = Role::OWNER;

        group->ownerId = owner->id;

        auto result = groups_m.addGroup(group);
        members_m.addMember(result->id, owner);

        return createDtoResponse(Status::CODE_200, result);
    }

    ENDPOINT("PATCH", "groups/group", updateGroup, QUERY(oatpp::Int64, id), BODY_DTO(Object<GroupDTO>, group), AUTHORIZATION(std::shared_ptr<handler::DefaultBearerAuthorizationObject>, auth))
    {
        auto requestee = users_m.getUserByAuth(auth->token);
        OATPP_ASSERT_HTTP(requestee != nullptr, Status::CODE_401, "Unauthorized");

        auto member = members_m.getMemberById(id, requestee->id);
        OATPP_ASSERT_HTTP(member != nullptr, Status::CODE_401, "Unauthorized");
        OATPP_ASSERT_HTTP(member->userRole == Role::OWNER, Status::CODE_401, "Unauthorized");

        auto result = groups_m.updateGroup(id, group);
        OATPP_ASSERT_HTTP(requestee != nullptr, Status::CODE_404, "Not found");

        return createDtoResponse(Status::CODE_200, result);
    }

    ENDPOINT("DELETE", "groups/deleteGroup", removeGroupById, QUERY(oatpp::Int64, id), AUTHORIZATION(std::shared_ptr<handler::DefaultBearerAuthorizationObject>, auth))
    {
        auto requestee = users_m.getUserByAuth(auth->token);
        OATPP_ASSERT_HTTP(requestee != nullptr, Status::CODE_401, "Unauthorized");

        auto member = members_m.getMemberById(id, requestee->id);
        OATPP_ASSERT_HTTP(member != nullptr, Status::CODE_401, "Unauthorized");
        OATPP_ASSERT_HTTP(member->userRole == Role::OWNER, Status::CODE_401, "Unauthorized");

        auto resultB = members_m.removeMemberById(id, requestee->id);
        auto resultC = groups_m.removeGroupById(id);

        return createDtoResponse(Status::CODE_200, resultC);
    }

    ENDPOINT("GET", "groups/{groupId}", getInfo, PATH(oatpp::Int64, groupId), AUTHORIZATION(std::shared_ptr<handler::DefaultBearerAuthorizationObject>, auth))
    {
        auto requestee = users_m.getUserByAuth(auth->token);
        OATPP_ASSERT_HTTP(requestee != nullptr, Status::CODE_401, "Unauthorized");
        OATPP_ASSERT_HTTP(members_m.getMemberById(groupId, requestee->id) != nullptr, Status::CODE_401, "Unauthorized");

        auto result = groups_m.getInfo(groupId);
        OATPP_ASSERT_HTTP(result != nullptr, Status::CODE_404, "Not found");

        return createDtoResponse(Status::CODE_200, result);
    }

    ENDPOINT("POST", "groups/{groupId}/member", addMember, PATH(oatpp::Int64, groupId), BODY_DTO(Object<MemberDTO>, member), AUTHORIZATION(std::shared_ptr<handler::DefaultBearerAuthorizationObject>, auth))
    {
        auto requestee = users_m.getUserByAuth(auth->token);
        OATPP_ASSERT_HTTP(requestee != nullptr, Status::CODE_401, "Unauthorized");

        auto result = members_m.getMemberById(groupId, requestee->id);
        OATPP_ASSERT_HTTP(result != nullptr, Status::CODE_401, "Unauthorized");
        OATPP_ASSERT_HTTP(result->userRole <= Role::ADMIN, Status::CODE_401, "Unauthorized");

        return createDtoResponse(Status::CODE_200, members_m.addMember(groupId, member));
    }

    ENDPOINT("DELETE", "groups/{groupId}/deleteMember", removeMemberById, PATH(oatpp::Int64, groupId), QUERY(Int64, id), AUTHORIZATION(std::shared_ptr<handler::DefaultBearerAuthorizationObject>, auth))
    {
        auto requestee = users_m.getUserByAuth(auth->token);
        OATPP_ASSERT_HTTP(requestee != nullptr, Status::CODE_401, "Unauthorized");

        auto result = members_m.getMemberById(groupId, requestee->id);
        OATPP_ASSERT_HTTP(result != nullptr, Status::CODE_401, "Unauthorized");
        OATPP_ASSERT_HTTP(result->userRole <= Role::ADMIN, Status::CODE_401, "Unauthorized");

        return createDtoResponse(Status::CODE_200, members_m.removeMemberById(groupId, id));
    }

    ENDPOINT("GET", "groups/{groupId}/member", getMemberById, PATH(oatpp::Int64, groupId), QUERY(Int64, id), AUTHORIZATION(std::shared_ptr<handler::DefaultBearerAuthorizationObject>, auth))
    {
        auto requestee = users_m.getUserByAuth(auth->token);
        OATPP_ASSERT_HTTP(requestee != nullptr, Status::CODE_401, "Unauthorized");
        OATPP_ASSERT_HTTP(members_m.getMemberById(groupId, requestee->id) != nullptr, Status::CODE_401, "Unauthorized");

        if (id == -1)
        {
            return createDtoResponse(Status::CODE_200, members_m.getMembers(groupId));
        }
        else
        {
            auto result = members_m.getMemberById(groupId, id);
            OATPP_ASSERT_HTTP(result != nullptr, Status::CODE_404, "Not found");
            return createDtoResponse(Status::CODE_200, result);
        }
    }

    ENDPOINT("POST", "groups/{groupId}/activity", addActivity, PATH(oatpp::Int64, groupId), BODY_DTO(Object<ActivityDTO>, activity), AUTHORIZATION(std::shared_ptr<handler::DefaultBearerAuthorizationObject>, auth))
    {
        auto requestee = users_m.getUserByAuth(auth->token);
        OATPP_ASSERT_HTTP(requestee != nullptr, Status::CODE_401, "Unauthorized");

        auto result = members_m.getMemberById(groupId, requestee->id);
        OATPP_ASSERT_HTTP(result != nullptr, Status::CODE_401, "Unauthorized");
        OATPP_ASSERT_HTTP(result->userRole <= Role::ADMIN, Status::CODE_401, "Unauthorized");

        return createDtoResponse(Status::CODE_200, activities_m.addActivity(groupId, activity));
    }

    ENDPOINT("PATCH", "groups/{groupId}/activity", updateActivity, PATH(oatpp::Int64, groupId), QUERY(Int64, id), BODY_DTO(Object<ActivityDTO>, activity), AUTHORIZATION(std::shared_ptr<handler::DefaultBearerAuthorizationObject>, auth))
    {
        auto requestee = users_m.getUserByAuth(auth->token);
        OATPP_ASSERT_HTTP(requestee != nullptr, Status::CODE_401, "Unauthorized");

        auto member = members_m.getMemberById(groupId, requestee->id);
        OATPP_ASSERT_HTTP(member != nullptr, Status::CODE_401, "Unauthorized");
        OATPP_ASSERT_HTTP(member->userRole <= Role::ADMIN, Status::CODE_401, "Unauthorized");

        auto result = activities_m.updateActivity(groupId, id, activity);
        OATPP_ASSERT_HTTP(requestee != nullptr, Status::CODE_404, "Not found");

        return createDtoResponse(Status::CODE_200, result);
    }

    ENDPOINT("DELETE", "groups/{groupId}/deleteActivity", removeActivityById, PATH(oatpp::Int64, groupId), QUERY(Int64, id), AUTHORIZATION(std::shared_ptr<handler::DefaultBearerAuthorizationObject>, auth))
    {
        auto requestee = users_m.getUserByAuth(auth->token);
        OATPP_ASSERT_HTTP(requestee != nullptr, Status::CODE_401, "Unauthorized");

        auto member = members_m.getMemberById(groupId, requestee->id);
        OATPP_ASSERT_HTTP(member != nullptr, Status::CODE_401, "Unauthorized");
        OATPP_ASSERT_HTTP(member->userRole <= Role::ADMIN, Status::CODE_401, "Unauthorized");

        auto result = activities_m.removeActivityById(groupId, id);
        OATPP_ASSERT_HTTP(requestee != nullptr, Status::CODE_404, "Not found");

        return createDtoResponse(Status::CODE_200, result);
    }

    ENDPOINT("GET", "groups/{groupId}/activity", getActivityById, PATH(oatpp::Int64, groupId), QUERY(Int64, id), AUTHORIZATION(std::shared_ptr<handler::DefaultBearerAuthorizationObject>, auth))
    {
        auto requestee = users_m.getUserByAuth(auth->token);
        OATPP_ASSERT_HTTP(requestee != nullptr, Status::CODE_401, "Unauthorized");
        OATPP_ASSERT_HTTP(members_m.getMemberById(groupId, requestee->id) != nullptr, Status::CODE_401, "Unauthorized");

        if (id == -1)
        {
            return createDtoResponse(Status::CODE_200, activities_m.getActivities(groupId));
        }
        else
        {
            auto result = activities_m.getActivityById(groupId, id);
            OATPP_ASSERT_HTTP(result != nullptr, Status::CODE_404, "Not found");
            return createDtoResponse(Status::CODE_200, result);
        }
    }
};

#include OATPP_CODEGEN_END(ApiController)
