#pragma once

#include "Group/Entities/Member/MemberService.hpp"

#include "User/UserService.hpp"

#include <oatpp/web/server/api/ApiController.hpp>
#include <oatpp/macro/codegen.hpp>
#include <oatpp/macro/component.hpp>

#include <oatpp/base/Log.hpp>

using namespace oatpp::web::server;
using namespace oatpp::network;

#include OATPP_CODEGEN_BEGIN(ApiController)

class MemberController : public api::ApiController
{
private:
    MemberService members_m;
    UserService users_m;

public:
    MemberController(OATPP_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, objectMapper))
        : api::ApiController(objectMapper)
    {
        setDefaultAuthorizationHandler(std::make_shared<handler::BearerAuthorizationHandler>(""));
    }

    ENDPOINT("POST", "groups/{groupId}/member", addMember, PATH(oatpp::Int64, groupId), BODY_DTO(Object<MemberDTO>, member), AUTHORIZATION(std::shared_ptr<handler::DefaultBearerAuthorizationObject>, auth))
    {
        auto requestee = users_m.getUserByAuth(auth->token);
        OATPP_ASSERT_HTTP(requestee != nullptr, Status::CODE_401, "Unauthorized");

        auto result = members_m.getMemberById(groupId, requestee->value->id);
        OATPP_ASSERT_HTTP(result != nullptr, Status::CODE_401, "Unauthorized");
        OATPP_ASSERT_HTTP(result->value->userRole <= Role::ADMIN, Status::CODE_401, "Unauthorized");

        return createDtoResponse(Status::CODE_200, members_m.addMember(groupId, member));
    }

    ENDPOINT("DELETE", "groups/{groupId}/deleteMember", removeMemberById, PATH(oatpp::Int64, groupId), QUERY(Int64, id), AUTHORIZATION(std::shared_ptr<handler::DefaultBearerAuthorizationObject>, auth))
    {
        auto requestee = users_m.getUserByAuth(auth->token);
        OATPP_ASSERT_HTTP(requestee != nullptr, Status::CODE_401, "Unauthorized");

        auto result = members_m.getMemberById(groupId, requestee->value->id);
        OATPP_ASSERT_HTTP(result != nullptr, Status::CODE_401, "Unauthorized");
        OATPP_ASSERT_HTTP(result->value->userRole <= Role::ADMIN, Status::CODE_401, "Unauthorized");

        return createDtoResponse(Status::CODE_200, members_m.removeMemberById(groupId, id));
    }

    ENDPOINT("GET", "groups/{groupId}/member", getMemberById, PATH(oatpp::Int64, groupId), QUERY(Int64, id), AUTHORIZATION(std::shared_ptr<handler::DefaultBearerAuthorizationObject>, auth))
    {
        auto requestee = users_m.getUserByAuth(auth->token);
        OATPP_ASSERT_HTTP(requestee != nullptr, Status::CODE_401, "Unauthorized");
        OATPP_ASSERT_HTTP(members_m.getMemberById(groupId, requestee->value->id) != nullptr, Status::CODE_401, "Unauthorized");

        if (id == -1)
        {
            return createDtoResponse(Status::CODE_200, members_m.getMembers(groupId));
        }
        else
        {
            return createDtoResponse(Status::CODE_200, members_m.getMemberById(groupId, id));
        }
    }
};

#include OATPP_CODEGEN_END(ApiController)
