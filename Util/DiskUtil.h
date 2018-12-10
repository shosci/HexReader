#pragma once

#include <string>
#include <vector>

namespace HexReader
{
    std::vector<std::wstring> GetLocalLogicalDrives() noexcept;

    std::vector<std::wstring> GetLocalPhysicalDrives() noexcept;
}