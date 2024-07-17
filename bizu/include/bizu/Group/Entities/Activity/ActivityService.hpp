#pragma once

#include "Generic/DTO/StatusDTO.hpp"

#include "ActivityRepository.hpp"
#include "DTO/ActivityDTO.hpp"

#include <oatpp/web/protocol/http/Http.hpp>
#include <oatpp/macro/component.hpp>

class ActivityService
{
private:
    OATPP_COMPONENT(std::shared_ptr<ActivityRepository>, repository_m);

public:
    oatpp::Object<ActivityDTO> addActivity(oatpp::Int64 const& groupId, oatpp::Object<ActivityDTO> const& dto) { return repository_m->addActivity(groupId, dto); }
    oatpp::Object<StatusDTO> removeActivityById(oatpp::Int64 const& groupId, oatpp::Int64 const& id) { return repository_m->removeActivityById(groupId, id); }
    oatpp::Object<ActivityDTO> getActivityById(oatpp::Int64 const& groupId, oatpp::Int64 const& id) { return repository_m->getActivityById(groupId, id); }
    oatpp::Vector<oatpp::Object<ActivityDTO>> getActivities(oatpp::Int64 groupId) { return repository_m->getActivities(groupId); }
};
