#include "User/UserController.hpp"
#include "Group/GroupController.hpp"

#include "MainComponent.hpp"

#include <oatpp/network/Server.hpp>

using namespace oatpp::web::server;
using namespace oatpp::network;

void run()
{
    [[maybe_unused]] MainComponent components;

    OATPP_COMPONENT(std::shared_ptr<HttpRouter>, router);
    {
        router->addController(std::make_shared<UserController>());
        router->addController(std::make_shared<GroupController>());
    }

    OATPP_COMPONENT(std::shared_ptr<ConnectionHandler>, connectionHandler);
    OATPP_COMPONENT(std::shared_ptr<ServerConnectionProvider>, connectionProvider);

    Server server { connectionProvider, connectionHandler };

    OATPP_LOGi("bizu: ", "Server running on port: {}", connectionProvider->getProperty("port").toString());
    server.run();

    OATPP_COMPONENT(std::shared_ptr<oatpp::provider::Provider<oatpp::sqlite::Connection>>, dbConnectionProvider);
    dbConnectionProvider->stop();
}

int main()
{
    oatpp::Environment::init();
    run();
    oatpp::Environment::destroy();
}
