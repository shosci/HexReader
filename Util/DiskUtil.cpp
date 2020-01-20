#include "DiskUtil.h"

#define UNICODE

#include <windows.h>
#include <fileapi.h>

#include <algorithm>
#include <iostream>
#include <sstream>
#include <wchar.h>

#include <setupapi.h>
#include <Ntddstor.h>

#include <memory>

#pragma comment( lib, "setupapi.lib" )

namespace HexReader
{
    //
    // logic drive is in form of C:, D:, etc, no trailing backslash
    //
    std::vector<std::wstring> GetLocalLogicalDrives() noexcept
    {
        std::vector<std::wstring> drives {};
        wchar_t tmpb[1] { L'\0' };
        auto strLength = GetLogicalDriveStringsW(1, tmpb);
        if(strLength == 0)
        {
            std::cout << "GetLogicalDriveStringsW failed to get needed buffer length" << std::endl;
            return drives;
        }
        
        int neededLength = strLength;
        wchar_t* buffer = new wchar_t[neededLength];
        strLength = GetLogicalDriveStringsW(neededLength, buffer);
        if(strLength == 0)
        {
            std::cout << "GetLogicalDriveStringsW failed to get drive strings" << std::endl;
            return drives;
        }

        // buffer would be in form of "C:\NULLD:\NULLNULL"
        wchar_t* sentinal = &buffer[neededLength-1];
        wchar_t* bufcpy = buffer;
        while(bufcpy <= sentinal)
        {
            auto itemStrLength = wcslen(bufcpy);
            if(itemStrLength == 0)
                break;

            if(bufcpy[itemStrLength-1] == L'\\')            
                drives.push_back(std::wstring(bufcpy, itemStrLength-1));
            else
                drives.push_back(std::wstring(bufcpy));

            bufcpy += itemStrLength+1;
        }

        delete[] buffer;

        return drives;
    }

    class DeviceInterfaceDetailedData
    {
    public:
        DeviceInterfaceDetailedData(std::uint32_t requiredSize) noexcept
        {
            _pDeviceInterfaceDetailData = ( PSP_DEVICE_INTERFACE_DETAIL_DATA ) malloc( requiredSize );
            SecureZeroMemory( _pDeviceInterfaceDetailData, requiredSize );
            _pDeviceInterfaceDetailData->cbSize = sizeof( PSP_DEVICE_INTERFACE_DETAIL_DATA );
        }

        ~DeviceInterfaceDetailedData()
        {
            free(_pDeviceInterfaceDetailData);
        }

        PSP_DEVICE_INTERFACE_DETAIL_DATA GetRawPtr() noexcept
        {
            return _pDeviceInterfaceDetailData;
        }

        const wchar_t* GetDevicePath() const noexcept
        {
            return (wchar_t*)_pDeviceInterfaceDetailData->DevicePath;
        }

    private:
        PSP_DEVICE_INTERFACE_DETAIL_DATA _pDeviceInterfaceDetailData { nullptr };

    };

    std::vector<std::wstring> GetLocalPhysicalDrives() noexcept
    {
        std::vector<std::wstring> drives;

        HDEVINFO hDiskDeviceInfoSet;
        GUID guidDiskDeviceInterface = GUID_DEVINTERFACE_DISK;

        //
        // The SetupDiGetClassDevs function returns a handle to a <i>device information set</i> that contains requested device information elements for a local computer.
        // https://docs.microsoft.com/en-us/windows-hardware/drivers/install/device-information-sets
        //
        hDiskDeviceInfoSet = SetupDiGetClassDevs( &guidDiskDeviceInterface,                // *ClassGuid
                                                  NULL,                                    // Enumerator
                                                  NULL,                                    // hwndParent
                                                  DIGCF_PRESENT | DIGCF_DEVICEINTERFACE ); // Flags

        if( INVALID_HANDLE_VALUE == hDiskDeviceInfoSet )
        {
            std::cout << "SetupDiGetClassDevs INVALID_HANDLE_VALUE. Last Error Code: " << static_cast<std::uint32_t>( GetLastError() ) << std::endl;
            return drives;
        }

        //
        // https://docs.microsoft.com/en-us/windows/desktop/api/setupapi/nf-setupapi-setupdienumdeviceinterfaces
        // The SetupDiEnumDeviceInterfaces function enumerates the device interfaces that are contained in a device information set.
        //
        SP_DEVICE_INTERFACE_DATA deviceInterfaceData;
        SecureZeroMemory( &deviceInterfaceData, sizeof( SP_DEVICE_INTERFACE_DATA ) );
        deviceInterfaceData.cbSize = sizeof( SP_DEVICE_INTERFACE_DATA );
        DWORD deviceIndex = 0;

        for(;
            SetupDiEnumDeviceInterfaces( hDiskDeviceInfoSet,       // DeviceInfoSet               
                                         NULL,                     // DeviceInfoData
                                         &guidDiskDeviceInterface, // InterfaceClassGuid
                                         deviceIndex,              // MemberIndex
                                         &deviceInterfaceData )    // DeviceInterfaceData
            ;
            deviceIndex++ )
        {
            // 
            // Get RequiredSize of the DeviceInterfaceDetailData
            //
            DWORD requiredSize = 0;
            BOOL fResult = SetupDiGetDeviceInterfaceDetail( hDiskDeviceInfoSet,   // DeviceInfoSet
                                                            &deviceInterfaceData, // DeviceInterfaceData
                                                            NULL,                 // DeviceInterfaceDetailData
                                                            0,                    // DeviceInterfaceDetailDataSize
                                                            &requiredSize,        // RequiredSize
                                                            NULL );               // DeviceInfoData

            //
            // https://docs.microsoft.com/en-us/windows/desktop/api/setupapi/nf-setupapi-setupdigetdeviceinterfacedetailw#remarks
            // Get the required buffer size.
            // Call SetupDiGetDeviceInterfaceDetail with a NULL DeviceInterfaceDetailData pointer, a DeviceInterfaceDetailDataSize of zero, and a valid RequiredSize variable.
            // In response to such a call, this function returns the required buffer size at RequiredSize and fails with GetLastError returning ERROR_INSUFFICIENT_BUFFER. 
            //
            if( GetLastError() != ERROR_INSUFFICIENT_BUFFER )
            {
                std::cout << "SetupDiGetDeviceInterfaceDetail failed to get RequiredSize for DeviceInterfaceDetailData. Last Error Code: " << static_cast<std::uint32_t>( GetLastError() ) << std::endl;
                return drives;
            }

            //
            // https://docs.microsoft.com/en-us/windows/desktop/api/setupapi/ns-setupapi-_sp_device_interface_detail_data_w
            //
            //
            // Get DeviceInterfaceDetailData
            //
            std::unique_ptr<DeviceInterfaceDetailedData> pDeviceInterfaceDetailData = std::make_unique<DeviceInterfaceDetailedData>( requiredSize );

            fResult = SetupDiGetDeviceInterfaceDetail( hDiskDeviceInfoSet,                      // DeviceInfoSet
                                                       &deviceInterfaceData,                    // DeviceInterfaceData
                                                       pDeviceInterfaceDetailData->GetRawPtr(), // DeviceInterfaceDetailData
                                                       requiredSize,                            // DeviceInterfaceDetailDataSize
                                                       NULL,                                    // RequiredSize
                                                       NULL );                                  // DeviceInfoData
            if( !fResult )
            {
                std::cout << "SetupDiGetDeviceInterfaceDetail failed to get DeviceInterfaceDetailData. Last Error Code: " << static_cast<std::uint32_t>( GetLastError() ) << std::endl;
                return drives;
            }

            std::wstring devicePath = pDeviceInterfaceDetailData->GetDevicePath();
            // std::wcout << "DevicePath from DeviceInterfaceDetailData: " << devicePath << std::endl;

            HANDLE diskHandle = CreateFileW( pDeviceInterfaceDetailData->GetDevicePath(), // lpFileName
                                            0,                                // dwDesiredAccess
                                            FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE,                            // dwShareMode
                                            NULL,                                        // lpSecurityAttributes
                                            OPEN_EXISTING,                               // dwCreationDisposition
                                            FILE_ATTRIBUTE_NORMAL,                       // dwFlagsAndAttributes
                                            NULL );                                      // hTemplateFile
            if( INVALID_HANDLE_VALUE == diskHandle )
            {
                std::cout << "CreateFile INVALID_HANDLE_VALUE. Last Error Code: " << static_cast<std::uint32_t>( GetLastError() ) << std::endl;
                return drives;
            }

            STORAGE_DEVICE_NUMBER deviceNumber;
            DWORD bytesReturned = 0;

            //
            // https://docs.microsoft.com/en-us/windows/desktop/api/winioctl/ni-winioctl-ioctl_storage_get_device_number#remarks
            // The values in the STORAGE_DEVICE_NUMBER structure are guaranteed to remain unchanged until the device is removed or the system is restarted.
            // It is not guaranteed to be persistent across device restarts or system restarts.
            //
            fResult = DeviceIoControl( diskHandle,                      // handle to device
                                       IOCTL_STORAGE_GET_DEVICE_NUMBER, // dwIoControlCode
                                       NULL,                            // lpInBuffer
                                       0,                               // nInBufferSize              
                                       &deviceNumber,                   // lpOutBuffer
                                       sizeof( STORAGE_DEVICE_NUMBER ), // nOutBufferSize    
                                       &bytesReturned,                  // lpBytesReturned   
                                       NULL );                          // lpOverlapped

            if( !fResult )
            {
                std::cout << "DeviceIoControl failed to get STORAGE_DEVICE_NUMBER. Last Error Code: " << static_cast<std::uint32_t>( GetLastError() ) << std::endl;
                return drives;
            }

            // std::cout << "DeviceType: " << deviceNumber.DeviceType << ";\tDeviceNumber: " << deviceNumber.DeviceNumber << ";\tPartitionNumber: " << deviceNumber.PartitionNumber << std::endl;

            drives.push_back(std::wstring(L"\\\\.\\PhysicalDrive") + std::to_wstring(deviceNumber.DeviceNumber));

            CloseHandle( diskHandle );
            diskHandle = INVALID_HANDLE_VALUE;
        }

        std::sort(drives.begin(), drives.end());
        return drives;
    }

    //
    // logicalDrive should be in form of "C:" or  Or "\\.\D:"
    //
    // return PhycicalDrive string in form of \\.\PhysicalDriveNUM
    std::wstring GetPhysicalDriveForLogicalDrive(const std::wstring& logicalDrive) noexcept
    {
        if(logicalDrive.size() == 0)
        {
            std::cout << "logicalDrive should not be empty " << std::endl;
            return {};
        }

        std::wstring nomalizedLogicalDrive;
        if(logicalDrive.substr(0,1) == L"\\")
            nomalizedLogicalDrive = logicalDrive;
        else
            nomalizedLogicalDrive = L"\\\\.\\" + logicalDrive;

        HANDLE hDevice = INVALID_HANDLE_VALUE;    // handle to the drive to be examined 
        BOOL bResult   = FALSE;                   // results flag
        DWORD junk     = 0;                       // discard results

        hDevice = CreateFileW( nomalizedLogicalDrive.c_str(),
                                0,                // no access to the drive
                                FILE_SHARE_READ | // share mode
                                FILE_SHARE_WRITE, 
                                NULL,             // default security attributes
                                OPEN_EXISTING,    // disposition
                                0,                // file attributes
                                NULL);            // do not copy file attributes

        if (hDevice == INVALID_HANDLE_VALUE)    // cannot open the drive
        {
            std::cout << "CreateFileW failed " << std::endl;
            return {};
        }

        VOLUME_DISK_EXTENTS vde;
        bResult = DeviceIoControl( hDevice,
                                   IOCTL_VOLUME_GET_VOLUME_DISK_EXTENTS,
                                   NULL /*lpInBuffer*/, 
                                   0 /*nInBufferSize*/,    
                                   &vde,
                                   sizeof(vde),
                                   &junk,
                                   (LPOVERLAPPED) NULL /*lpOverlapped*/);

        CloseHandle(hDevice);

        if(bResult == 0)
        {
            std::cout << "DeviceIoControl failed " << std::endl;
            return {};
        }
        
        if(vde.NumberOfDiskExtents <= 0)
        {
            std::wcout << L"There is no disk extents for drive: " << logicalDrive << std::endl;
            return {};
        }

        DISK_EXTENT fstExt = vde.Extents[0];

        std::wostringstream woss;
        woss << L"\\\\.\\PhysicalDrive" << fstExt.DiskNumber;

        return woss.str();
    }
}
