#pragma once

#include "Group/DTO/GroupDTO.hpp"

#include <oatpp-sqlite/orm.hpp>
#include <oatpp/base/Log.hpp>

#include OATPP_CODEGEN_BEGIN(DbClient)

class GroupDAO : public oatpp::orm::DbClient
{
public:
    GroupDAO(std::shared_ptr<oatpp::orm::Executor> const& executor)
        : oatpp::orm::DbClient(executor)
    {}

    QUERY(
        addGroup,
        "INSERT INTO groups (name, ownerId) VALUES (:group.name, :group.ownerId);",
        PARAM(oatpp::Object<GroupDTO>, group)
    );

    QUERY(
        removeGroupById,
        "DELETE FROM groups WHERE id = :groupId;",
        PARAM(oatpp::Int64, groupId)
    );

    QUERY(
        getGroupById,
        "SELECT * FROM groups WHERE id = :groupId;",
        PARAM(oatpp::Int64, groupId)
    );

    QUERY(
        getGroups,
        "SELECT * FROM groups WHERE id IN (SELECT groupId FROM groupMembers WHERE userId = :memberId);",
        PARAM(oatpp::Int64, memberId)
    );

    QUERY(
        getName,
        "SELECT name FROM groups WHERE id = :groupId;",
        PARAM(oatpp::Int64, groupId)
    );

    QUERY(
        updateGroupName,
        "UPDATE groups "
        "    SET name = :name "
        "WHERE id = :groupId;",
        PARAM(oatpp::Int64, groupId),
        PARAM(oatpp::String, name)
    );
};

#include OATPP_CODEGEN_END(DbClient)
