#include "User/UserRepository.hpp"

#include <fmt/core.h>
#include <oatpp/web/protocol/http/Http.hpp>

using namespace oatpp::web::protocol::http;

Operation<oatpp::Object<UserDTO>> UserRepository::addUser(oatpp::Object<UserDTO> const& user)
{
    auto status = Operation<oatpp::Object<UserDTO>>::createShared();

    auto operation = dao_m.getObject()->addUser(user);
    if (operation->isSuccess())
    {
        status->value = getUserById(oatpp::sqlite::Utils::getLastInsertRowId(operation->getConnection()))->value;
        status->status = "OK";
        status->code = 200;
    }
    else
    {
        status->status = fmt::format("SQL OPERATION ERROR: {}", *operation->getErrorMessage());
        status->code = 500;
    }

    return status;
}

Operation<void> UserRepository::removeUserById(oatpp::Int64 const& userId)
{
    auto status = Operation<void>::createShared();

    if (getUserById(userId)->code == 200)
    {
        auto const operation = dao_m.getObject()->removeUserById(userId);
        if (operation->isSuccess())
        {
            status->status = "OK";
            status->code = 200;
        }
        else
        {
            status->status = fmt::format("SQL OPERATION FAILURE: {}", *operation->getErrorMessage());
            status->code = 500;
        }
    }
    else
    {
        status->status = "USER NOT FOUND";
        status->code = 404;
    }

    return status;
}

Operation<void> UserRepository::updateUser(oatpp::Int64 const& userId, oatpp::Object<UserDTO> const& user)
{
    auto status = Operation<void>::createShared();

    if (getUserById(userId)->code == 200)
    {
        if (user->name)
        {
            auto const operation = dao_m.getObject()->updateUserName(userId, user->name);
            if (!operation->isSuccess())
            {
                status->status = fmt::format("SQL OPERATION FAILURE: {}", *operation->getErrorMessage());
                status->code = 500;
                return status;
            }
        }

        if (user->password)
        {
            auto const operation = dao_m.getObject()->updateUserPassword(userId, user->password);
            if (!operation->isSuccess())
            {
                status->status = fmt::format("SQL OPERATION FAILURE: {}", *operation->getErrorMessage());
                status->code = 500;
                return status;
            }
        }

        status->status = "OK";
        status->code = 200;
    }
    else
    {
        status->status = "USER NOT FOUND";
        status->code = 404;
    }

    return status;
}

Operation<oatpp::Vector<oatpp::Object<UserDTO>>> UserRepository::getUsers()
{
    auto status = Operation<oatpp::Vector<oatpp::Object<UserDTO>>>::createShared();

    auto const operation = dao_m.getObject()->getUsers();
    if (operation->isSuccess())
    {
        status->value = operation->fetch<oatpp::Vector<oatpp::Object<UserDTO>>>();
        status->status = "OK";
        status->code = 200;
    }
    else
    {
        status->status = fmt::format("SQL OPERATION FAILURE: {}", *operation->getErrorMessage());
        status->code = 500;
    }

    return status;
}

Operation<oatpp::Object<UserDTO>> UserRepository::getUser(oatpp::Object<UserDTO> const& user)
{
    auto status = Operation<oatpp::Object<UserDTO>>::createShared();

    auto const operation = dao_m.getObject()->getUser(user);
    if (operation->isSuccess())
    {
        if (operation->hasMoreToFetch())
        {
            status->status = "OK";
            status->code = 200;
            status->value = operation->fetch<oatpp::Vector<oatpp::Object<UserDTO>>>()->at(0);
        }
        else
        {
            status->status = "USER NOT FOUND";
            status->code = 404;
        }
    }
    else
    {
        status->status = fmt::format("SQL OPERATION FAILURE: {}", *operation->getErrorMessage());
        status->code = 500;
    }

    return status;
}

Operation<oatpp::Object<UserDTO>> UserRepository::getUserById(oatpp::Int64 const& userId)
{
    auto status = Operation<oatpp::Object<UserDTO>>::createShared();

    auto const operation = dao_m.getObject()->getUserById(userId);
    if (operation->isSuccess())
    {
        if (operation->hasMoreToFetch())
        {
            status->status = "OK";
            status->code = 200;
            status->value = operation->fetch<oatpp::Vector<oatpp::Object<UserDTO>>>()->at(0);
        }
        else
        {
            status->status = "USER NOT FOUND";
            status->code = 404;
        }
    }
    else
    {
        status->status = fmt::format("SQL OPERATION ERROR", *operation->getErrorMessage());
        status->code = 500;
    }

    return status;
}

Operation<oatpp::Object<UserDTO>> UserRepository::getUserByAuth(oatpp::String const& auth)
{
    auto status = Operation<oatpp::Object<UserDTO>>::createShared();

    auto operation = dao_m.getObject()->getUserByAuth(auth);
    if (operation->isSuccess())
    {
        if (operation->hasMoreToFetch())
        {
            status->status = "OK";
            status->code = 200;
            status->value = operation->fetch<oatpp::Vector<oatpp::Object<UserDTO>>>()->at(0);
        }
        else
        {
            status->status = "USER NOT FOUND";
            status->code = 404;
        }
    }
    else
    {
        status->status = fmt::format("SQL OPERATION ERROR: {}", *operation->getErrorMessage());
        status->code = 500;
    }

    return status;
}

