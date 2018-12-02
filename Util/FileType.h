// File utility class
#pragma once

#include <string>
namespace HexReader
{

enum class FileType
{
    Unknown = 0,
    File = 1,
    Directory = 2,
    Disk = 3,
};

FileType GetFileType(std::string& filePath) noexcept;

std::string GetPrettyFileType(FileType fileType) noexcept;

} // HexReader
