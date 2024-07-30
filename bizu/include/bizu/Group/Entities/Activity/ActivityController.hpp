#pragma once

#include "Group/Entities/Activity/ActivityService.hpp"

#include "Group/Entities/Member/DTO/MemberDTO.hpp"
#include "User/UserService.hpp"
#include "Group/Entities/Member/MemberService.hpp"
#include "oatpp/web/protocol/http/Http.hpp"

#include <oatpp/web/server/api/ApiController.hpp>
#include <oatpp/macro/codegen.hpp>
#include <oatpp/macro/component.hpp>

#include <oatpp/base/Log.hpp>

using namespace oatpp::web::server;
using namespace oatpp::network;

#include OATPP_CODEGEN_BEGIN(ApiController)

class ActivityController : public api::ApiController
{
private:
    ActivityService activities_m;
    UserService users_m;
    MemberService members_m;

public:
    ActivityController(OATPP_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, objectMapper))
        : api::ApiController(objectMapper)
    {
        setDefaultAuthorizationHandler(std::make_shared<handler::BearerAuthorizationHandler>(""));
    }

    ENDPOINT("POST", "groups/{groupId}/activity", addActivity, PATH(oatpp::Int64, groupId), BODY_DTO(Object<ActivityDTO>, activity), AUTHORIZATION(std::shared_ptr<handler::DefaultBearerAuthorizationObject>, auth))
    {
        auto requestee = users_m.getUserByAuth(auth->token);
        OATPP_ASSERT_HTTP(requestee != nullptr, Status::CODE_401, "Unauthorized");

        auto result = members_m.getMemberById(groupId, requestee->value->id);
        OATPP_ASSERT_HTTP(result != nullptr, Status::CODE_401, "Unauthorized");
        OATPP_ASSERT_HTTP(result->value->userRole <= Role::ADMIN, Status::CODE_401, "Unauthorized");

        return createDtoResponse(Status::CODE_200, activities_m.addActivity(groupId, activity));
    }

    ENDPOINT("PATCH", "groups/{groupId}/activity", updateActivity, PATH(oatpp::Int64, groupId), QUERY(Int64, id), BODY_DTO(Object<ActivityDTO>, activity), AUTHORIZATION(std::shared_ptr<handler::DefaultBearerAuthorizationObject>, auth))
    {
        auto requestee = users_m.getUserByAuth(auth->token);
        OATPP_ASSERT_HTTP(requestee != nullptr, Status::CODE_401, "Unauthorized");

        auto member = members_m.getMemberById(groupId, requestee->value->id);
        OATPP_ASSERT_HTTP(member != nullptr, Status::CODE_401, "Unauthorized");
        OATPP_ASSERT_HTTP(member->value->userRole <= Role::ADMIN, Status::CODE_401, "Unauthorized");

        return createDtoResponse(Status::CODE_200, activities_m.updateActivity(groupId, id, activity));
    }

    ENDPOINT("DELETE", "groups/{groupId}/deleteActivity", removeActivityById, PATH(oatpp::Int64, groupId), QUERY(Int64, id), AUTHORIZATION(std::shared_ptr<handler::DefaultBearerAuthorizationObject>, auth))
    {
        auto requestee = users_m.getUserByAuth(auth->token);
        OATPP_ASSERT_HTTP(requestee != nullptr, Status::CODE_401, "Unauthorized");

        auto member = members_m.getMemberById(groupId, requestee->value->id);
        OATPP_ASSERT_HTTP(member != nullptr, Status::CODE_401, "Unauthorized");
        OATPP_ASSERT_HTTP(member->value->userRole <= Role::ADMIN, Status::CODE_401, "Unauthorized");

        return createDtoResponse(Status::CODE_200, activities_m.removeActivityById(groupId, id));
    }

    ENDPOINT("GET", "groups/{groupId}/activity", getActivityById, PATH(oatpp::Int64, groupId), QUERY(Int64, id), AUTHORIZATION(std::shared_ptr<handler::DefaultBearerAuthorizationObject>, auth))
    {
        auto requestee = users_m.getUserByAuth(auth->token);
        OATPP_ASSERT_HTTP(requestee != nullptr, Status::CODE_401, "Unauthorized");
        OATPP_ASSERT_HTTP(members_m.getMemberById(groupId, requestee->value->id) != nullptr, Status::CODE_401, "Unauthorized");

        if (id == -1)
        {
            return createDtoResponse(Status::CODE_200, activities_m.getActivities(groupId));
        }
        else
        {
            return createDtoResponse(Status::CODE_200, activities_m.getActivityById(groupId, id));
        }
    }
};

#include OATPP_CODEGEN_END(ApiController)
