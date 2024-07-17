#pragma once

#include "DatabaseClient.hpp"

#include <cassert>

#include <oatpp/macro/component.hpp>
#include <sqlite3.h>

class DatabaseComponent
{
public:
    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::provider::Provider<oatpp::sqlite::Connection>>, dbConnectionProvider)([] {
        auto connectionProvider = std::make_shared<oatpp::sqlite::ConnectionProvider>("/home/nyyakko/Git/bizu/database.sqlite");
        return oatpp::sqlite::ConnectionPool::createShared(connectionProvider, 10, std::chrono::seconds(5));
    }());

    OATPP_CREATE_COMPONENT(std::shared_ptr<DatabaseClient>, databaseClient)([]
    {
        OATPP_COMPONENT(std::shared_ptr<oatpp::provider::Provider<oatpp::sqlite::Connection>>, dbConnectionProvider);
        auto executor = std::make_shared<oatpp::sqlite::Executor>(dbConnectionProvider);
        auto client = std::make_shared<DatabaseClient>(executor);
        return client;
    }());
};
