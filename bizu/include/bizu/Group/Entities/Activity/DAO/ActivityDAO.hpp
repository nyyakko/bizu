#pragma once

#include "Group/Entities/Activity/DTO/ActivityDTO.hpp"

#include <oatpp-sqlite/orm.hpp>
#include <oatpp/base/Log.hpp>

#include OATPP_CODEGEN_BEGIN(DbClient)

class ActivityDAO : public oatpp::orm::DbClient
{
public:
    ActivityDAO(std::shared_ptr<oatpp::orm::Executor> const& executor)
        : oatpp::orm::DbClient(executor)
    {}

    QUERY(
        addActivity,
        "INSERT INTO groupActivities (subject, bimester, category, due, description, groupId)"
        "   VALUES (:activity.subject, :activity.bimester, :activity.category, :activity.due, :activity.description, :groupId);",
        PARAM(oatpp::Int64, groupId),
        PARAM(oatpp::Object<ActivityDTO>, activity)
    );

    QUERY(
        removeActivityById,
        "DELETE FROM groupActivities WHERE groupId = :groupId AND id = :id;",
        PARAM(oatpp::Int64, groupId),
        PARAM(oatpp::Int64, id)
    );

    QUERY(
        getActivityById,
        "SELECT * FROM groupActivities WHERE groupId = :groupId AND id = :id;",
        PARAM(oatpp::Int64, groupId),
        PARAM(oatpp::Int64, id)
    );

    QUERY(
        getActivities,
        "SELECT * FROM groupActivities WHERE groupId = :groupId;",
        PARAM(oatpp::Int64, groupId)
    );

    QUERY(
        updateActivitySubject,
        "UPDATE groupActivities "
        "    SET subject = :subject "
        "WHERE groupId = :groupId AND id = :activityId;",
        PARAM(oatpp::Int64, groupId),
        PARAM(oatpp::Int64, activityId),
        PARAM(oatpp::String, subject)
    );

    QUERY(
        updateActivityBimester,
        "UPDATE groupActivities "
        "    SET bimester = :bimester "
        "WHERE groupId = :groupId AND id = :activityId;",
        PARAM(oatpp::Int64, groupId),
        PARAM(oatpp::Int64, activityId),
        PARAM(oatpp::Int8, bimester)
    );

    QUERY(
        updateActivityCategory,
        "UPDATE groupActivities "
        "    SET category = :category "
        "WHERE groupId = :groupId AND id = :activityId;",
        PARAM(oatpp::Int64, groupId),
        PARAM(oatpp::Int64, activityId),
        PARAM(oatpp::Int8, category)
    );

    QUERY(
        updateActivityDueDate,
        "UPDATE groupActivities "
        "    SET due = :due "
        "WHERE groupId = :groupId AND id = :activityId;",
        PARAM(oatpp::Int64, groupId),
        PARAM(oatpp::Int64, activityId),
        PARAM(oatpp::Int64, due)
    );

    QUERY(
        updateActivityDescription,
        "UPDATE groupActivities "
        "    SET description = :description "
        "WHERE groupId = :groupId AND id = :activityId;",
        PARAM(oatpp::Int64, groupId),
        PARAM(oatpp::Int64, activityId),
        PARAM(oatpp::String, description)
    );
};

#include OATPP_CODEGEN_END(DbClient)
