#pragma once

#include <oatpp/Types.hpp>
#include <oatpp/macro/codegen.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

class GroupInviteDTO : public oatpp::DTO
{
    DTO_INIT(GroupInviteDTO, DTO);

    DTO_FIELD(String, value);
};

#include OATPP_CODEGEN_END(DTO)
