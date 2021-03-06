/* A program to read file, disk data and print in hex format
*  Following is an example(512 byte, 16 byte per line):
*      00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F         ................
*      10 11 12 13 14 15 16 17 18 19 1A 1B 1C 1D 1E 1F         ................
*      20 21 22 23 24 25 26 27 28 29 2A 2B 2C 2D 2E 2F         ................
*      30 31 32 33 34 35 36 37 38 39 3A 3B 3C 3D 3E 3F         ................
*      40 41 42 43 44 45 46 47 48 49 4A 4B 4C 4D 4E 4F         .ABCDEFGHIJKLMNO
*      50 51 52 53 54 55 56 57 58 59 5A 5B 5C 5D 5E 5F         PQRSTUVWXYZ.....
*      60 61 62 63 64 65 66 67 68 69 6A 6B 6C 6D 6E 6F         .abcdefghijklmno
*      70 71 72 73 74 75 76 77 78 79 7A 7B 7C 7D 7E 7F         pqrstuvwxyz.....
*      80 81 82 83 84 85 86 87 88 89 8A 8B 8C 8D 8E 8F         ................
*      90 91 92 93 94 95 96 97 98 99 9A 9B 9C 9D 9E 9F         ................
*      A0 A1 A2 A3 A4 A5 A6 A7 A8 A9 AA AB AC AD AE AF         ................
*      B0 B1 B2 B3 B4 B5 B6 B7 B8 B9 BA BB BC BD BE BF         ................
*      C0 C1 C2 C3 C4 C5 C6 C7 C8 C9 CA CB CC CD CE CF         ................
*      D0 D1 D2 D3 D4 D5 D6 D7 D8 D9 DA DB DC DD DE DF         ................
*      E0 E1 E2 E3 E4 E5 E6 E7 E8 E9 EA EB EC ED EE EF         ................
*      F0 F1 F2 F3 F4 F5 F6 F7 F8 F9 FA FB FC FD FE FF         ................
*      00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F         ................
*      10 11 12 13 14 15 16 17 18 19 1A 1B 1C 1D 1E 1F         ................
*      20 21 22 23 24 25 26 27 28 29 2A 2B 2C 2D 2E 2F         ................
*      30 31 32 33 34 35 36 37 38 39 3A 3B 3C 3D 3E 3F         ................
*      40 41 42 43 44 45 46 47 48 49 4A 4B 4C 4D 4E 4F         .ABCDEFGHIJKLMNO
*      50 51 52 53 54 55 56 57 58 59 5A 5B 5C 5D 5E 5F         PQRSTUVWXYZ.....
*      60 61 62 63 64 65 66 67 68 69 6A 6B 6C 6D 6E 6F         .abcdefghijklmno
*      70 71 72 73 74 75 76 77 78 79 7A 7B 7C 7D 7E 7F         pqrstuvwxyz.....
*      80 81 82 83 84 85 86 87 88 89 8A 8B 8C 8D 8E 8F         ................
*      90 91 92 93 94 95 96 97 98 99 9A 9B 9C 9D 9E 9F         ................
*      A0 A1 A2 A3 A4 A5 A6 A7 A8 A9 AA AB AC AD AE AF         ................
*      B0 B1 B2 B3 B4 B5 B6 B7 B8 B9 BA BB BC BD BE BF         ................
*      C0 C1 C2 C3 C4 C5 C6 C7 C8 C9 CA CB CC CD CE CF         ................
*      D0 D1 D2 D3 D4 D5 D6 D7 D8 D9 DA DB DC DD DE DF         ................
*      E0 E1 E2 E3 E4 E5 E6 E7 E8 E9 EA EB EC ED EE EF         ................
*      F0 F1 F2 F3 F4 F5 F6 F7 F8 F9 FA FB FC FD FE FF         ................
*/

#include "DiskUtil.h"
#include "DiskReader.h"

#include <windows.h>

#include <fstream>
#include <iostream>
#include <type_traits>

int main(int argc, wchar_t* argv[])
{
    // auto logicalDrives = HexReader::GetLocalLogicalDrives();

    // for(auto drive : logicalDrives)
    // {
    //     std::wcout << L"Volume: " << drive << std::endl;
    //     std::wcout << L"PhysicalDrive: " << HexReader::GetPhysicalDriveForLogicalDrive(drive) << std::endl;       
    // }

    // constexpr uint32_t size = 512;
    // HexReader::BYTE bytes[size] {0};
    // int i{0};
    // for(auto& b: bytes)
    //     b = i++;

    // HexReader::PrettyPrint(bytes, size);

    // std::cout << std::endl;

    // for(int i=0; i<argc; i++)
    //     std::cout << std::string(argv[i]) << std::endl;
    //
    // output:
    // D:\Projects\HexReader>Bin\HexReader.exe arg1 arg2 arg3
    // Bin\HexReader.exe
    // arg1
    // arg2
    // arg3

    std::wstring filePath;
    if(argc < 2)
    {
        std::cout << "HexReader: " << std::endl
            << "\t\tYou can use this tool to read a file or device in bytes with Hex Format" << std::endl
            << "\t\tIf it's a file, put its absolute file path." << std::endl
            << "\t\tIf it's a device, just put its volume char and end with colon(no backslash), like: C:, D: or E:" << std::endl
            << std::endl
            << "Please input your file or device:";
        
        std::wcin >> filePath;
    }
    else
    {
        filePath = argv[1];
    }

    std::wcout << L"Opening: " << filePath << L" ..." << std::endl;

    HexReader::DiskReader dr(HexReader::GetPhysicalDriveForLogicalDrive(filePath));

    while(std::getchar() != 'q')
    {
        if(dr.PrintNext(512))
            std::wcout << std::endl << L"Press q to quit, any key to continue: " << std::endl;
        else
        {
            std::wcout << L"EOF..." << std::endl;
            break;
        }
    }

    return 0;
}