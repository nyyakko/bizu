#pragma once

#include "Generic/DTO/OperationDTO.hpp"

#include "UserRepository.hpp"
#include "DTO/UserDTO.hpp"

#include <oatpp/web/protocol/http/Http.hpp>
#include <oatpp/macro/component.hpp>

class UserService
{
private:
    OATPP_COMPONENT(std::shared_ptr<UserRepository>, repository_m);

public:
    Operation<oatpp::Object<UserDTO>> addUser(oatpp::Object<UserDTO> const& user) { return repository_m->addUser(user); }
    Operation<void> updateUser(oatpp::Int64 const& userId, oatpp::Object<UserDTO> const& user) { return repository_m->updateUser(userId, user); }
    Operation<void> removeUserById(oatpp::Int64 const& userId) { return repository_m->removeUserById(userId); }
    Operation<oatpp::Vector<oatpp::Object<UserDTO>>> getUsers() { return repository_m->getUsers(); }
    Operation<oatpp::Object<UserDTO>> getUser(oatpp::Object<UserDTO> const& user) { return repository_m->getUser(user); }
    Operation<oatpp::Object<UserDTO>> getUserById(oatpp::Int64 const& userId) { return repository_m->getUserById(userId); }
    Operation<oatpp::Object<UserDTO>> getUserByAuth(oatpp::String const& authorization) { return repository_m->getUserByAuth(authorization); }
};
