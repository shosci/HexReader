#pragma once

#include <cinttypes>

namespace HexReader
{

    using BYTE = char;
    void PrettyPrint(BYTE* pBytes, uint32_t size) noexcept;

}