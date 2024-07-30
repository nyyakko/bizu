#include "Group/Entities/Activity/ActivityRepository.hpp"

#include <array>
#include <fmt/core.h>
#include <fmt/format.h>
#include <fplus/search.hpp>
#include <oatpp/base/Log.hpp>

Operation<oatpp::Object<ActivityDTO>> ActivityRepository::addActivity(oatpp::Int64 const& groupId, oatpp::Object<ActivityDTO> const& activity)
{
    auto status = Operation<oatpp::Object<ActivityDTO>>::createShared();

    auto const operation = dao_m.getObject()->addActivity(groupId, activity);
    if (operation->isSuccess())
    {
        status->value = getActivityById(groupId, oatpp::sqlite::Utils::getLastInsertRowId(operation->getConnection()))->value;
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

Operation<void> ActivityRepository::updateActivity(oatpp::Int64 const& groupId, oatpp::Int64 const& activityId, oatpp::Object<ActivityDTO> const& activity)
{
    auto status = Operation<void>::createShared();

    if (getActivityById(groupId, activityId)->code == 200)
    {
        std::array<std::shared_ptr<oatpp::orm::QueryResult>, 5> queries {};

        if (activity->subject) queries.at(0) = dao_m.getObject()->updateActivitySubject(groupId, activityId, activity->subject);
        if (activity->bimester) queries.at(1) = dao_m.getObject()->updateActivityBimester(groupId, activityId, activity->bimester);
        if (activity->category) queries.at(2) = dao_m.getObject()->updateActivityCategory(groupId, activityId, activity->category);
        if (activity->due) queries.at(3) = dao_m.getObject()->updateActivityDueDate(groupId, activityId, activity->due);
        if (activity->description) queries.at(4) = dao_m.getObject()->updateActivityDescription(groupId, activityId, activity->description);

        if (auto maybeFailed =
                fplus::find_first_by([] (auto&& query) {
                    return !query->isSuccess();
                }, queries); maybeFailed.is_nothing())
        {
            status->status = "OK";
            status->code = 200;
        }
        else
        {
            status->status = fmt::format("SQL EXECUTION FAILURE: {}", *maybeFailed.get_with_default(nullptr)->getErrorMessage());
            status->code = 500;
        }
    }
    else
    {
        status->status = "ACTIVITY NOT FOUND";
        status->code = 404;
    }

    return status;
}

Operation<void> ActivityRepository::removeActivityById(oatpp::Int64 const& groupId, oatpp::Int64 const& activityId)
{
    auto status = Operation<void>::createShared();

    if (getActivityById(groupId, activityId)->code == 200)
    {
        auto const operation = dao_m.getObject()->removeActivityById(groupId, activityId);
        if (operation->isSuccess())
        {
            status->status = "OK";
            status->code = 200;
        }
        else
        {
            status->status = fmt::format("SQL EXECUTION FAILURE: {}", *operation->getErrorMessage());
            status->code = 500;
        }
    }
    else
    {
        status->status = "ACTIVITY NOT FOUND";
        status->code = 404;
    }

    return status;
}

Operation<oatpp::Object<ActivityDTO>> ActivityRepository::getActivityById(oatpp::Int64 const& groupId, oatpp::Int64 const& activityId)
{
    auto status = Operation<oatpp::Object<ActivityDTO>>::createShared();

    auto const operation = dao_m.getObject()->getActivityById(groupId, activityId);
    if (operation->isSuccess())
    {
        if (operation->hasMoreToFetch())
        {
            status->value = operation->fetch<oatpp::Vector<oatpp::Object<ActivityDTO>>>()->at(0);
            status->status = "OK";
            status->code = 200;
        }
        else
        {
            status->status = "ACTIVITY NOT FOUND";
            status->code = 404;
        }
    }
    else
    {
        status->status = fmt::format("SQL EXECUTION FAILURE: {}", *operation->getErrorMessage());
        status->code = 500;
    }

    return status;
}

Operation<oatpp::Vector<oatpp::Object<ActivityDTO>>> ActivityRepository::getActivities(oatpp::Int64 const& groupId)
{
    auto status = Operation<oatpp::Vector<oatpp::Object<ActivityDTO>>>::createShared();

    auto const operation = dao_m.getObject()->getActivities(groupId);
    if (operation->isSuccess())
    {
        status->value = operation->fetch<oatpp::Vector<oatpp::Object<ActivityDTO>>>();
        status->status = "OK";
        status->code = 200;
    }
    else
    {
        status->status = fmt::format("SQL EXECUTION FAILURE: {}", *operation->getErrorMessage());
        status->code = 500;
    }

    return status;
}
