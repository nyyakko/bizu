#pragma once

#include <oatpp/Types.hpp>
#include <oatpp/macro/codegen.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

class ActivityDTO : public oatpp::DTO
{
    DTO_INIT(ActivityDTO, DTO);

    DTO_FIELD(Int64, id);
    DTO_FIELD(String, subject);
    DTO_FIELD(Int8, bimester);
    DTO_FIELD(Int8, category);
    DTO_FIELD(Int64, due);
    DTO_FIELD(String, description);
    DTO_FIELD(Int64, groupId);
};

#include OATPP_CODEGEN_END(DTO)
