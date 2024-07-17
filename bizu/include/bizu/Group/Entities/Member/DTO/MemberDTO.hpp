#pragma once

#include <oatpp/Types.hpp>
#include <oatpp/macro/codegen.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

ENUM(Role, v_int64,
    VALUE(OWNER, 0, "ROLE_OWNER"),
    VALUE(ADMIN, 1, "ROLE_ADMIN"),
    VALUE(GUEST, 2, "ROLE_GUEST")
);

class MemberDTO : public oatpp::DTO
{
    DTO_INIT(MemberDTO, DTO);

    DTO_FIELD(Int64, id);
    DTO_FIELD(String, name);
    DTO_FIELD(Enum<Role>::AsString, userRole);
};

#include OATPP_CODEGEN_END(DTO)
