#pragma once

#include "Group/Entities/Member/DTO/MemberDTO.hpp"

#include <oatpp-sqlite/orm.hpp>
#include <oatpp/base/Log.hpp>

#include OATPP_CODEGEN_BEGIN(DbClient)

class MemberDAO : public oatpp::orm::DbClient
{
public:
    MemberDAO(std::shared_ptr<oatpp::orm::Executor> const& executor)
        : oatpp::orm::DbClient(executor)
    {}

    QUERY(
        addMember,
        "INSERT INTO groupMembers (userId, userRole, groupId) VALUES (:member.id, :member.userRole, :groupId);",
        PARAM(oatpp::Int64, groupId),
        PARAM(oatpp::Object<MemberDTO>, member)
    );

    QUERY(
        removeMemberById,
        "DELETE FROM groupMembers WHERE groupId = :groupId AND userId = :memberId;",
        PARAM(oatpp::Int64, groupId),
        PARAM(oatpp::Int64, memberId)
    );

    QUERY(
        getMemberById,
        "SELECT users.id, users.name, groupMembers.userRole FROM users "
            "INNER JOIN groupMembers ON groupMembers.userId = users.id "
            "WHERE groupMembers.groupId = :groupId AND users.id = :memberId;",
        PARAM(oatpp::Int64, groupId),
        PARAM(oatpp::Int64, memberId)
    );

    QUERY(
        getMembers,
        "SELECT users.id, users.name, groupMembers.userRole FROM users "
            "INNER JOIN groupMembers ON groupMembers.userId = users.id "
            "WHERE groupMembers.groupId = :groupId;",
        PARAM(oatpp::Int64, groupId)
    );
};

#include OATPP_CODEGEN_END(DbClient)
