#pragma once

#include <oatpp/Types.hpp>
#include <oatpp/macro/codegen.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

class GroupDTO : public oatpp::DTO
{
    DTO_INIT(GroupDTO, DTO);

    DTO_FIELD(Int64, id);
    DTO_FIELD(String, name);
    DTO_FIELD(Int64, ownerId);
};

#include OATPP_CODEGEN_END(DTO)
