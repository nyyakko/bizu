#pragma once

#include "Generic/DTO/OperationDTO.hpp"

#include "DTO/ActivityDTO.hpp"
#include "DAO/ActivityDAO.hpp"

#include <oatpp/Types.hpp>
#include <oatpp/web/protocol/http/Http.hpp>
#include <oatpp/macro/component.hpp>

class ActivityRepository
{
private:
    OATPP_CREATE_COMPONENT(std::shared_ptr<ActivityDAO>, dao_m)([] {
        OATPP_COMPONENT(std::shared_ptr<oatpp::provider::Provider<oatpp::sqlite::Connection>>, dbConnectionProvider);
        auto executor = std::make_shared<oatpp::sqlite::Executor>(dbConnectionProvider);
        return std::make_shared<ActivityDAO>(executor);
    }());

public:
    Operation<oatpp::Object<ActivityDTO>> addActivity(oatpp::Int64 const& groupId, oatpp::Object<ActivityDTO> const& activity);
    Operation<void> updateActivity(oatpp::Int64 const& groupId, oatpp::Int64 const& activityId, oatpp::Object<ActivityDTO> const& activity);
    Operation<void> removeActivityById(oatpp::Int64 const& groupId, oatpp::Int64 const& activityId);
    Operation<oatpp::Object<ActivityDTO>> getActivityById(oatpp::Int64 const& groupId, oatpp::Int64 const& activityId);
    Operation<oatpp::Vector<oatpp::Object<ActivityDTO>>> getActivities(oatpp::Int64 const& groupId);
};

