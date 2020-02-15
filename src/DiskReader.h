#pragma once

#include <cinttypes>
#include <string>

#include <windows.h>

namespace HexReader
{
    class DiskReader
    {
        unsigned char* _pBuffer;
        const uint32_t _nBufferSize;
        uint32_t       _currentPos;
        bool           _isEOF {false};
        HANDLE         _hDevice {INVALID_HANDLE_VALUE};
    public:
        explicit DiskReader(std::wstring physicalDrive);
        explicit DiskReader(std::wstring physicalDrive, uint32_t bufferSize);

        ~DiskReader() noexcept;

        // return false when it reaches the start
        // bool PrintPrevious(uint32_t byteCount);

        // return false when it reaches the end
        bool PrintNext(uint32_t byteCount);

        bool DiskReader::IsBufferEmpty() const noexcept;

    private:
        DiskReader( const DiskReader& diskReader) = delete;
        DiskReader& operator=( const DiskReader& diskReader) = delete;
    };
}