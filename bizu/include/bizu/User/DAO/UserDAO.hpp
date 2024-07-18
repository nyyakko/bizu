#pragma once

#include "User/DTO/UserDTO.hpp"

#include <oatpp-sqlite/orm.hpp>
#include <oatpp/base/Log.hpp>

#include OATPP_CODEGEN_BEGIN(DbClient)

class UserDAO : public oatpp::orm::DbClient
{
public:
    UserDAO(std::shared_ptr<oatpp::orm::Executor> const& executor)
        : oatpp::orm::DbClient(executor)
    {}

    QUERY(
        addUser,
        "INSERT INTO users (name, password, auth)"
        "   VALUES (:user.name, :user.password, :user.auth);",
        PARAM(oatpp::Object<UserDTO>, user)
    );

    QUERY(
        removeUserById,
        "DELETE FROM users WHERE id = :id;",
        PARAM(oatpp::Int64, id)
    );

    QUERY(
        getUsers,
        "SELECT * FROM users;"
    );

    QUERY(
        getUser,
        "SELECT * FROM users WHERE name = :user.name AND password = :user.password;",
        PARAM(oatpp::Object<UserDTO>, user)
    );

    QUERY(
        getUserById,
        "SELECT * FROM users WHERE id = :id;",
        PARAM(oatpp::Int64, id)
    );

    QUERY(
        getUserByAuth,
        "SELECT * FROM users WHERE auth = :auth;",
        PARAM(oatpp::String, auth)
    );

    QUERY(
        updateUserName,
        "UPDATE users "
        "    SET name = :name "
        "WHERE id = :id;",
        PARAM(oatpp::Int64, id),
        PARAM(oatpp::String, name)
    );

    QUERY(
        updateUserPassword,
        "UPDATE users "
        "    SET password = :password "
        "WHERE id = :id;",
        PARAM(oatpp::Int64, id),
        PARAM(oatpp::String, password)
    );
};

#include OATPP_CODEGEN_END(DbClient)
