#pragma once

#include "Group/GroupService.hpp"

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

class GroupController : public api::ApiController
{
private:
    GroupService groups_m;
    UserService users_m;
    MemberService members_m;

public:
    GroupController(OATPP_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, objectMapper))
        : api::ApiController(objectMapper)
    {
        setDefaultAuthorizationHandler(std::make_shared<handler::BearerAuthorizationHandler>(""));
    }

    ENDPOINT("POST", "groups/", addGroup, BODY_DTO(Object<GroupDTO>, group), AUTHORIZATION(std::shared_ptr<handler::DefaultBearerAuthorizationObject>, auth))
    {
        auto requestee = users_m.getUserByAuth(auth->token);
        OATPP_ASSERT_HTTP(requestee != nullptr, Status::CODE_401, "Unauthorized");

        auto owner = MemberDTO::createShared();
        owner->name = requestee->value->name;
        owner->id = requestee->value->id;
        owner->userRole = Role::OWNER;

        group->ownerId = owner->id;

        auto const result = groups_m.addGroup(group);
        if (result->code == 200)
        {
            members_m.addMember(result->value->id, owner);
        }

        return createDtoResponse(Status::CODE_200, result);
    }

    ENDPOINT("PATCH", "groups/group", updateGroup, QUERY(oatpp::Int64, id), BODY_DTO(Object<GroupDTO>, group), AUTHORIZATION(std::shared_ptr<handler::DefaultBearerAuthorizationObject>, auth))
    {
        auto requestee = users_m.getUserByAuth(auth->token);
        OATPP_ASSERT_HTTP(requestee != nullptr, Status::CODE_401, "Unauthorized");

        auto member = members_m.getMemberById(id, requestee->value->id);
        OATPP_ASSERT_HTTP(member != nullptr, Status::CODE_401, "Unauthorized");
        OATPP_ASSERT_HTTP(member->value->userRole == Role::OWNER, Status::CODE_401, "Unauthorized");

        return createDtoResponse(Status::CODE_200, groups_m.updateGroup(id, group));
    }

    ENDPOINT("DELETE", "groups/deleteGroup", removeGroupById, QUERY(oatpp::Int64, id), AUTHORIZATION(std::shared_ptr<handler::DefaultBearerAuthorizationObject>, auth))
    {
        auto requestee = users_m.getUserByAuth(auth->token);
        OATPP_ASSERT_HTTP(requestee != nullptr, Status::CODE_401, "Unauthorized");

        auto member = members_m.getMemberById(id, requestee->value->id);
        OATPP_ASSERT_HTTP(member != nullptr, Status::CODE_401, "Unauthorized");
        OATPP_ASSERT_HTTP(member->value->userRole == Role::OWNER, Status::CODE_401, "Unauthorized");

        return createDtoResponse(Status::CODE_200, groups_m.removeGroupById(id));
    }

    ENDPOINT("GET", "groups/", getGroups, QUERY(oatpp::Int64, id), AUTHORIZATION(std::shared_ptr<handler::DefaultBearerAuthorizationObject>, auth))
    {
        auto requestee = users_m.getUserByAuth(auth->token);
        OATPP_ASSERT_HTTP(requestee != nullptr, Status::CODE_401, "Unauthorized");

        if (id == -1)
        {
            return createDtoResponse(Status::CODE_200, groups_m.getGroups(requestee->value->id));
        }
        else
        {
            OATPP_ASSERT_HTTP(members_m.getMemberById(id, requestee->value->id) != nullptr, Status::CODE_401, "Unauthorized");
            return createDtoResponse(Status::CODE_200, groups_m.getGroupInfo(id));
        }
    }

    ENDPOINT("POST", "groups/{groupId}/invite", createInvite, PATH(oatpp::Int64, groupId), AUTHORIZATION(std::shared_ptr<handler::DefaultBearerAuthorizationObject>, auth))
    {
        auto requestee = users_m.getUserByAuth(auth->token);
        OATPP_ASSERT_HTTP(requestee != nullptr, Status::CODE_401, "Unauthorized");

        auto member = members_m.getMemberById(groupId, requestee->value->id);
        OATPP_ASSERT_HTTP(member != nullptr, Status::CODE_401, "Unauthorized");
        OATPP_ASSERT_HTTP(member->value->userRole <= Role::ADMIN, Status::CODE_401, "Unauthorized");

        return createDtoResponse(Status::CODE_200, groups_m.createGroupInvite(groupId));
    }

    ENDPOINT("POST", "groups/invite", joinByInvite, QUERY(oatpp::String, key), AUTHORIZATION(std::shared_ptr<handler::DefaultBearerAuthorizationObject>, auth))
    {
        auto requestee = users_m.getUserByAuth(auth->token);
        OATPP_ASSERT_HTTP(requestee != nullptr, Status::CODE_401, "Unauthorized");

        auto const group = groups_m.getGroupByInvite(key);

        if (group->code == 200)
        {
            OATPP_ASSERT_HTTP(members_m.getMemberById(group->value->id, requestee->value->id) == nullptr, Status::CODE_403, "Forbidden");

            auto member = MemberDTO::createShared();
            member->name = requestee->value->name;
            member->id = requestee->value->id;
            member->userRole = Role::GUEST;

            members_m.addMember(group->value->id, member);
        }

        return createDtoResponse(Status::CODE_200, group);
    }
};

#include OATPP_CODEGEN_END(ApiController)
