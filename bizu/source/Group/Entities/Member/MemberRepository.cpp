#include "Group/Entities/Member/MemberRepository.hpp"

#include <fmt/format.h>

Operation<oatpp::Object<MemberDTO>> MemberRepository::addMember(oatpp::Int64 const& groupId, oatpp::Object<MemberDTO> const& member)
{
    auto status = Operation<oatpp::Object<MemberDTO>>::createShared();

    auto const operation = dao_m.getObject()->addMember(groupId, member);
    if (operation->isSuccess())
    {
        status->value = getMemberById(groupId, oatpp::sqlite::Utils::getLastInsertRowId(operation->getConnection()))->value;
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

Operation<void> MemberRepository::removeMemberById(oatpp::Int64 const& groupId, oatpp::Int64 const& memberId)
{
    auto status = Operation<void>::createShared();

    auto const operation = dao_m.getObject()->removeMemberById(groupId, memberId);
    if (operation->isSuccess())
    {
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

Operation<oatpp::Vector<oatpp::Object<MemberDTO>>> MemberRepository::getMembers(oatpp::Int64 const& groupId)
{
    auto status = Operation<oatpp::Vector<oatpp::Object<MemberDTO>>>::createShared();

    auto const operation = dao_m.getObject()->getMembers(groupId);
    if (operation->isSuccess())
    {
        status->value = operation->fetch<oatpp::Vector<oatpp::Object<MemberDTO>>>();
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

Operation<oatpp::Object<MemberDTO>> MemberRepository::getMemberById(oatpp::Int64 const& groupId, oatpp::Int64 const& memberId)
{
    auto status = Operation<oatpp::Object<MemberDTO>>::createShared();

    auto const operation = dao_m.getObject()->getMemberById(groupId, memberId);
    if (operation->isSuccess())
    {
        if (operation->hasMoreToFetch())
        {
            status->value = operation->fetch<oatpp::Vector<oatpp::Object<MemberDTO>>>()->at(0);
            status->status = "OK";
            status->code = 200;
        }
        else
        {
            status->status = "MEMBER NOT FOUND";
            status->code = 404;
        }
    }
    else
    {
        status->status = fmt::format("SQL OPERATION FAILURE: {}", *operation->getErrorMessage());
        status->code = 500;
    }

    return status;
}

