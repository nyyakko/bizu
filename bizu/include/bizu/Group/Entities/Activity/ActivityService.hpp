#pragma once

#include "Generic/DTO/OperationDTO.hpp"

#include "ActivityRepository.hpp"
#include "DTO/ActivityDTO.hpp"

#include <oatpp/web/protocol/http/Http.hpp>
#include <oatpp/macro/component.hpp>

class ActivityService
{
private:
    OATPP_COMPONENT(std::shared_ptr<ActivityRepository>, repository_m);

public:
    Operation<oatpp::Object<ActivityDTO>> addActivity(oatpp::Int64 const& groupId, oatpp::Object<ActivityDTO> const& activity) { return repository_m->addActivity(groupId, activity); }
    Operation<void> updateActivity(oatpp::Int64 const& groupId, oatpp::Int64 const& activityId, oatpp::Object<ActivityDTO> const& activity) { return repository_m->updateActivity(groupId, activityId, activity); }
    Operation<void> removeActivityById(oatpp::Int64 const& groupId, oatpp::Int64 const& activityId) { return repository_m->removeActivityById(groupId, activityId); }
    Operation<oatpp::Object<ActivityDTO>> getActivityById(oatpp::Int64 const& groupId, oatpp::Int64 const& activityId) { return repository_m->getActivityById(groupId, activityId); }
    Operation<oatpp::Vector<oatpp::Object<ActivityDTO>>> getActivities(oatpp::Int64 groupId) { return repository_m->getActivities(groupId); }
};
