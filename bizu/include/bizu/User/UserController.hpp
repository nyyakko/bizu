#pragma once

#include "User/UserService.hpp"

#include <oatpp/web/server/api/ApiController.hpp>
#include <oatpp/macro/codegen.hpp>
#include <oatpp/macro/component.hpp>
#include <fmt/format.h>

using namespace oatpp::web::server;
using namespace oatpp::network;

#include OATPP_CODEGEN_BEGIN(ApiController)

class UserController : public api::ApiController
{
private:
    UserService users_m;

public:
    UserController(OATPP_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, objectMapper))
        : api::ApiController(objectMapper)
    {
        setDefaultAuthorizationHandler(std::make_shared<handler::BearerAuthorizationHandler>(""));
    }

    ENDPOINT("POST", "users/", addUser, BODY_DTO(Object<UserDTO>, user))
    {
        return createDtoResponse(Status::CODE_200, users_m.addUser(user));
    }

    ENDPOINT("PATCH", "users/user", updateUser, QUERY(Int64, id), BODY_DTO(oatpp::Object<UserDTO>, user), AUTHORIZATION(std::shared_ptr<handler::DefaultBearerAuthorizationObject>, auth))
    {
        auto requestee = users_m.getUserByAuth(auth->token);
        OATPP_ASSERT_HTTP(requestee->code == 200, Status::CODE_401, "Unauthorized");
        return createDtoResponse(Status::CODE_200, users_m.updateUser(id, user));
    }

    ENDPOINT("DELETE", "users/deleteUser", removeUserById, QUERY(Int64, id), AUTHORIZATION(std::shared_ptr<handler::DefaultBearerAuthorizationObject>, auth))
    {
        auto requestee = users_m.getUserByAuth(auth->token);
        OATPP_ASSERT_HTTP(requestee->code == 200, Status::CODE_401, "Unauthorized");
        return createDtoResponse(Status::CODE_200, users_m.removeUserById(id));
    }

    ENDPOINT("GET", "users/user", getUser, QUERY(String, name), QUERY(String, password))
    {
        auto user = UserDTO::createShared();
        user->name = name;
        user->password = password;
        return createDtoResponse(Status::CODE_200, users_m.getUser(user));
    }

    ENDPOINT("GET", "users/self", getSelf, AUTHORIZATION(std::shared_ptr<handler::DefaultBearerAuthorizationObject>, auth))
    {
        auto const requestee = users_m.getUserByAuth(auth->token);
        OATPP_ASSERT_HTTP(requestee->code == 200, Status::CODE_401, "Unauthorized");
        return createDtoResponse(Status::CODE_200, requestee);
    }
};

#include OATPP_CODEGEN_END(ApiController)
