#include "Group/Entities/Member/MemberRepository.hpp"

oatpp::Object<MemberDTO> MemberRepository::addMember(oatpp::Int64 const& groupId, oatpp::Object<MemberDTO> const& member)
{
    auto operation = dao_m.getObject()->addMember(groupId, member);
    auto memberId = oatpp::sqlite::Utils::getLastInsertRowId(operation->getConnection());
    return getMemberById(groupId, static_cast<v_int32>(memberId));
}

oatpp::Object<StatusDTO> MemberRepository::removeMemberById(oatpp::Int64 const& groupId, oatpp::Int64 const& memberId)
{
    auto operation = dao_m.getObject()->removeMemberById(groupId, memberId);
    auto status = StatusDTO::createShared();
    status->status = "OK";
    status->code = 200;
    return status;
}

oatpp::Vector<oatpp::Object<MemberDTO>> MemberRepository::getMembers(oatpp::Int64 const& groupId)
{
    auto operation = dao_m.getObject()->getMembers(groupId);
    if (!operation->hasMoreToFetch()) return nullptr; // FIXME: nulls are scary
    return operation->fetch<oatpp::Vector<oatpp::Object<MemberDTO>>>();
}

oatpp::Object<MemberDTO> MemberRepository::getMemberById(oatpp::Int64 const& groupId, oatpp::Int64 const& memberId)
{
    auto operation = dao_m.getObject()->getMemberById(groupId, memberId);
    if (!operation->hasMoreToFetch()) return nullptr; // FIXME: nulls are scary
    auto result = operation->fetch<oatpp::Vector<oatpp::Object<MemberDTO>>>();
    return result->at(0);
}

