#include "FileType.h"

namespace HexReader
{
    FileType GetFileType(std::string& /*filePath*/) noexcept
    {
        FileType fileType = FileType::Unknown;

        return fileType;
    }

    std::string GetPrettyFileType(FileType fileType) noexcept
    {
        switch(fileType)
        {
            case FileType::File:
                return "File";
            case FileType::Directory:
                return "Directory";
            case FileType::Disk:
                return "Disk";
        }

        return "Unknown";
    }
}