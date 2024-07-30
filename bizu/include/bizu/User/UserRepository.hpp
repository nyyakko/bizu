#pragma once

#include "Generic/DTO/OperationDTO.hpp"

#include "DTO/UserDTO.hpp"
#include "DAO/UserDAO.hpp"

#include <oatpp/Types.hpp>
#include <oatpp/web/protocol/http/Http.hpp>
#include <oatpp/macro/component.hpp>

class UserRepository
{
private:
    OATPP_CREATE_COMPONENT(std::shared_ptr<UserDAO>, dao_m)([] {
        OATPP_COMPONENT(std::shared_ptr<oatpp::provider::Provider<oatpp::sqlite::Connection>>, dbConnectionProvider);
        auto executor = std::make_shared<oatpp::sqlite::Executor>(dbConnectionProvider);
        return std::make_shared<UserDAO>(executor);
    }());

public:
    Operation<oatpp::Object<UserDTO>> addUser(oatpp::Object<UserDTO> const& user);
    Operation<void> updateUser(oatpp::Int64 const& userId, oatpp::Object<UserDTO> const& user);
    Operation<void> removeUserById(oatpp::Int64 const& userId);
    Operation<oatpp::Vector<oatpp::Object<UserDTO>>> getUsers();
    Operation<oatpp::Object<UserDTO>> getUser(oatpp::Object<UserDTO> const& user);
    Operation<oatpp::Object<UserDTO>> getUserById(oatpp::Int64 const& userId);
    Operation<oatpp::Object<UserDTO>> getUserByAuth(oatpp::String const& authorization);
};

