#pragma once

#include "User/UserService.hpp"

#include <oatpp/web/server/api/ApiController.hpp>
#include <oatpp/macro/codegen.hpp>
#include <oatpp/macro/component.hpp>

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
        auto result = users_m.addUser(user);
        OATPP_ASSERT_HTTP(result != nullptr, Status::CODE_404, "Not found");
        return createDtoResponse(Status::CODE_200, result);
    }

    ENDPOINT("GET", "users/user", getUser, QUERY(String, name), QUERY(String, password))
    {
        auto user = UserDTO::createShared();
        user->name = name;
        user->password = password;
        auto result = users_m.getUser(user);
        OATPP_ASSERT_HTTP(result != nullptr, Status::CODE_404, "Not found");
        return createDtoResponse(Status::CODE_200, result);
    }

    ENDPOINT("DELETE", "users/deleteUser", removeUserById, QUERY(Int64, id), AUTHORIZATION(std::shared_ptr<handler::DefaultBearerAuthorizationObject>, auth))
    {
        OATPP_ASSERT_HTTP(users_m.getUserByAuth(auth->token) != nullptr, Status::CODE_401, "Unauthorized");
        auto result = users_m.removeUserById(id);
        OATPP_ASSERT_HTTP(result != nullptr, Status::CODE_404, "Not found");
        return createDtoResponse(Status::CODE_200, result);
    }
};

#include OATPP_CODEGEN_END(ApiController)
