#include "User/UserRepository.hpp"

#include <oatpp/web/protocol/http/Http.hpp>

using namespace oatpp::web::protocol::http;

oatpp::Object<UserDTO> UserRepository::addUser(oatpp::Object<UserDTO> const& user)
{
    auto operation = dao_m.getObject()->addUser(user);
    auto activityId = oatpp::sqlite::Utils::getLastInsertRowId(operation->getConnection());
    return getUserById(static_cast<v_int32>(activityId));
}

oatpp::Object<StatusDTO> UserRepository::removeUserById(oatpp::Int64 id)
{
    auto operation = dao_m.getObject()->removeUserById(id);

    auto status = StatusDTO::createShared();
    status->status = "OK";
    status->code = 200;

    return status;
}

oatpp::Vector<oatpp::Object<UserDTO>> UserRepository::getUsers()
{
    auto operation = dao_m.getObject()->getUsers();
    if (!operation->hasMoreToFetch()) return nullptr; // FIXME: nulls are scary
    return operation->fetch<oatpp::Vector<oatpp::Object<UserDTO>>>();
}

oatpp::Object<UserDTO> UserRepository::getUser(oatpp::Object<UserDTO> const& user)
{
    auto operation = dao_m.getObject()->getUser(user);
    if (!operation->hasMoreToFetch()) return nullptr; // FIXME: nulls are scary
    auto result = operation->fetch<oatpp::Vector<oatpp::Object<UserDTO>>>();
    return result->at(0);
}

oatpp::Object<UserDTO> UserRepository::getUserById(oatpp::Int64 userId)
{
    auto operation = dao_m.getObject()->getUserById(userId);
    if (!operation->hasMoreToFetch()) return nullptr; // FIXME: nulls are scary
    auto result = operation->fetch<oatpp::Vector<oatpp::Object<UserDTO>>>();
    return result->at(0);
}

oatpp::Object<UserDTO> UserRepository::getUserByAuth(oatpp::String const& auth)
{
    auto operation = dao_m.getObject()->getUserByAuth(auth);
    if (!operation->hasMoreToFetch()) return nullptr; // FIXME: nulls are scary
    auto result = operation->fetch<oatpp::Vector<oatpp::Object<UserDTO>>>();
    return result->at(0);
}

