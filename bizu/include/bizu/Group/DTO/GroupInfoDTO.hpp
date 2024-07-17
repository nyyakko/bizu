#pragma once

#include "Group/Entities/Member/DTO/MemberDTO.hpp"
#include "Group/Entities/Activity/DTO/ActivityDTO.hpp"

#include <oatpp/Types.hpp>
#include <oatpp/macro/codegen.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

class GroupInfoDTO : public oatpp::DTO
{
    DTO_INIT(GroupInfoDTO, DTO);

    DTO_FIELD(Int64, id);
    DTO_FIELD(String, name);
    DTO_FIELD(Vector<oatpp::Object<MemberDTO>>, members);
    DTO_FIELD(Vector<oatpp::Object<ActivityDTO>>, activities);
};

#include OATPP_CODEGEN_END(DTO)
