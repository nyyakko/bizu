#pragma once

#include "Generic/DTO/StatusDTO.hpp"

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
    oatpp::Object<UserDTO> addUser(oatpp::Object<UserDTO> const& user);
    oatpp::Object<StatusDTO> removeUserById(oatpp::Int64 userId);
    oatpp::Vector<oatpp::Object<UserDTO>> getUsers();
    oatpp::Object<UserDTO> getUser(oatpp::Object<UserDTO> const& user);
    oatpp::Object<UserDTO> getUserById(oatpp::Int64 userId);
    oatpp::Object<UserDTO> getUserByAuth(oatpp::String const& authorization);
};

