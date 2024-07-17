#pragma once

#include "Generic/DTO/StatusDTO.hpp"

#include "UserRepository.hpp"
#include "DTO/UserDTO.hpp"

#include <oatpp/web/protocol/http/Http.hpp>
#include <oatpp/macro/component.hpp>

class UserService
{
private:
    OATPP_COMPONENT(std::shared_ptr<UserRepository>, repository_m);

public:
    oatpp::Object<UserDTO> addUser(oatpp::Object<UserDTO> const& user) { return repository_m->addUser(user); }
    oatpp::Object<StatusDTO> removeUserById(oatpp::Int64 const& userId) { return repository_m->removeUserById(userId); }
    oatpp::Vector<oatpp::Object<UserDTO>> getUsers() { return repository_m->getUsers(); }
    oatpp::Object<UserDTO> getUser(oatpp::Object<UserDTO> const& user) { return repository_m->getUser(user); }
    oatpp::Object<UserDTO> getUserById(oatpp::Int64 userId) { return repository_m->getUserById(userId); }
    oatpp::Object<UserDTO> getUserByAuth(oatpp::String const& authorization) { return repository_m->getUserByAuth(authorization); }
};
