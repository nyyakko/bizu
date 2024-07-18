#include "Group/Entities/Activity/ActivityRepository.hpp"

#include <oatpp/base/Log.hpp>

oatpp::Object<ActivityDTO> ActivityRepository::addActivity(oatpp::Int64 const& groupId, oatpp::Object<ActivityDTO> const& activity)
{
    auto operation = dao_m.getObject()->addActivity(groupId, activity);
    auto activityId = oatpp::sqlite::Utils::getLastInsertRowId(operation->getConnection());
    return getActivityById(groupId, static_cast<v_int32>(activityId));
}

oatpp::Object<StatusDTO> ActivityRepository::updateActivity(oatpp::Int64 const& groupId, oatpp::Int64 const& activityId, oatpp::Object<ActivityDTO> const& activity)
{
    if (getActivityById(groupId, activityId) == nullptr) return nullptr;

    if (activity->subject) dao_m.getObject()->updateActivitySubject(groupId, activityId, activity->subject);
    if (activity->bimester) dao_m.getObject()->updateActivityBimester(groupId, activityId, activity->bimester);
    if (activity->category) dao_m.getObject()->updateActivityCategory(groupId, activityId, activity->category);
    if (activity->due) dao_m.getObject()->updateActivityDueDate(groupId, activityId, activity->due);
    if (activity->description) dao_m.getObject()->updateActivityDescription(groupId, activityId, activity->description);

    auto status = StatusDTO::createShared();
    status->status = "OK";
    status->code = 200;
    return status;
}

oatpp::Object<StatusDTO> ActivityRepository::removeActivityById(oatpp::Int64 const& groupId, oatpp::Int64 const& activityId)
{
    if (getActivityById(groupId, activityId) == nullptr) return nullptr;

    auto operation = dao_m.getObject()->removeActivityById(groupId, activityId);
    auto status = StatusDTO::createShared();
    status->status = "OK";
    status->code = 200;
    return status;
}

oatpp::Object<ActivityDTO> ActivityRepository::getActivityById(oatpp::Int64 const& groupId, oatpp::Int64 const& activityId)
{
    auto operation = dao_m.getObject()->getActivityById(groupId, activityId);
    if (!operation->hasMoreToFetch()) return nullptr; // FIXME: nulls are scary
    auto result = operation->fetch<oatpp::Vector<oatpp::Object<ActivityDTO>>>();
    return result->at(0);
}

oatpp::Vector<oatpp::Object<ActivityDTO>> ActivityRepository::getActivities(oatpp::Int64 const& groupId)
{
    auto operation = dao_m.getObject()->getActivities(groupId);
    return operation->fetch<oatpp::Vector<oatpp::Object<ActivityDTO>>>();
}
