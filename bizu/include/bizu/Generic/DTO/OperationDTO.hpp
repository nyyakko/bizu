#pragma once

#include <oatpp/Types.hpp>
#include <oatpp/macro/codegen.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

template <class T>
struct OperationDTO : oatpp::DTO
{
    DTO_INIT(OperationDTO, DTO);

    DTO_FIELD(T, value);
    DTO_FIELD(String, status);
    DTO_FIELD(Int64, code);
};

template <>
struct OperationDTO<void> : oatpp::DTO
{
    DTO_INIT(OperationDTO, DTO);

    DTO_FIELD(String, status);
    DTO_FIELD(Int64, code);
};

template <class T>
using Operation = oatpp::Object<OperationDTO<T>>;

#include OATPP_CODEGEN_END(DTO)


