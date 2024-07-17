#pragma once

#include <oatpp/Types.hpp>
#include <oatpp/macro/codegen.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

struct StatusDTO : oatpp::DTO
{
    DTO_INIT(StatusDTO, DTO);

    DTO_FIELD(String, status);
    DTO_FIELD(Int64, code);
};

#include OATPP_CODEGEN_END(DTO)

