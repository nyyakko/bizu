#pragma once

#include "User/UserRepository.hpp"
#include "Group/GroupRepository.hpp"

#include <oatpp-sqlite/orm.hpp>
#include <oatpp/base/Log.hpp>
#include <oatpp/macro/component.hpp>

#include OATPP_CODEGEN_BEGIN(DbClient)

class DatabaseClient : public oatpp::orm::DbClient
{
public:
    DatabaseClient(std::shared_ptr<oatpp::orm::Executor> const& executor)
        : oatpp::orm::DbClient(executor)
    {
        oatpp::orm::SchemaMigration migration(executor);
        migration.addFile(1, HOME"/sql/script.sql");
        migration.migrate();
        auto version = executor->getSchemaVersion();
        OATPP_LOGd("DatabaseClient: ", "Version: {}", version);
    }

    OATPP_CREATE_COMPONENT(std::shared_ptr<GroupRepository>, groupRepository_m)([] {
        return std::make_shared<GroupRepository>();
    }());

    OATPP_CREATE_COMPONENT(std::shared_ptr<UserRepository>, userRepository_m)([] {
        return std::make_shared<UserRepository>();
    }());
};

#include OATPP_CODEGEN_END(DbClient)
