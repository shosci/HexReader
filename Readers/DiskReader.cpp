#include "DiskReader.h"

#include "..\Util\HexPrinter.h"

#include <algorithm>
#include <exception>

namespace HexReader
{
    constexpr uint32_t DeaultBufferSize = 8*1024*1024; // 8M

    DiskReader::DiskReader(std::wstring physicalDrive)
        : DiskReader(physicalDrive, DeaultBufferSize)
    {}

    DiskReader::DiskReader(std::wstring physicalDrive, uint32_t bufferSize)
        : _nBufferSize {bufferSize},
        _currentPos {bufferSize}
    {
        _pBuffer = new unsigned char[_nBufferSize];

        _hDevice = CreateFileW( physicalDrive.c_str(),
                                GENERIC_READ,     // no access to the drive
                                FILE_SHARE_READ,  // share mode
                                NULL,             // default security attributes
                                OPEN_EXISTING,    // disposition
                                0,                // file attributes
                                NULL);            // do not copy file attributes
        if( INVALID_HANDLE_VALUE == _hDevice)
            throw std::exception("failed to open this device");
    }

    DiskReader::~DiskReader() noexcept
    {
        delete[] _pBuffer;
        CloseHandle(_hDevice);
    }

    bool DiskReader::IsBufferEmpty() const noexcept
    {
        return _nBufferSize - _currentPos <= 0;
    }

    bool DiskReader::PrintNext(uint32_t byteCount)
    {
        if(IsBufferEmpty() && _isEOF)
            return false;

        uint32_t byteToPrint = byteCount;
        // check currentBuffer
        uint32_t bytesInBuffer = _nBufferSize - _currentPos;

        if(bytesInBuffer >= byteToPrint)
        {
            HexReader::PrettyPrint(_pBuffer + _currentPos, byteToPrint);
            _currentPos += byteToPrint;
            return true;
        }

        HexReader::PrettyPrint(_pBuffer + _currentPos, bytesInBuffer);

        byteToPrint -= bytesInBuffer;

        while(!_isEOF)
        {
            DWORD bytesRead {0};
            BOOL result = ReadFile(
                    _hDevice,
                    _pBuffer,
                    _nBufferSize,
                    &bytesRead,
                    NULL /*lpOverlapped*/
                );
            DWORD lastErr = GetLastError();
            HRESULT hr = HRESULT_FROM_WIN32(lastErr);
            if(result == 0 || FAILED(hr))
                throw std::exception("ReadFile failed for this device");
            
            if(bytesRead < _nBufferSize)
                _isEOF = true;
            
            _currentPos = std::min<uint32_t>(byteToPrint, static_cast<uint32_t>(bytesRead));

            HexReader::PrettyPrint(_pBuffer, _currentPos);

            byteToPrint -= _currentPos;

            if(byteToPrint <= 0)
                return true;
        }

        return false;
    }

}