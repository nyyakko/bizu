#pragma once

#include <oatpp/Types.hpp>
#include <oatpp/macro/codegen.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

class UserDTO : public oatpp::DTO
{
    DTO_INIT(UserDTO, DTO);

    DTO_FIELD(Int64, id);
    DTO_FIELD(String, name);
    DTO_FIELD(String, password);
    DTO_FIELD(String, auth);
};

#include OATPP_CODEGEN_END(DTO)
