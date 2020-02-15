#include "HexPrinter.h"

#include <iomanip>
#include <iostream>
#include <vector>

namespace HexReader
{
    static char GetPrintChar(char c) noexcept
    {
        if(('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z'))
            return c;
        return '.';
    }

    void PrettyPrint(unsigned char* pBytes, uint32_t size) noexcept
    {
        std::vector<char> curLine;
        for(int i = 0; i < size; i++)
        {
            std::cout << std::hex << std::uppercase << std::setw(2) << std::setfill('0') << static_cast<uint8_t>(*(pBytes+i))%256 << ' ';
            curLine.push_back(GetPrintChar(*(pBytes+i)));
            if((i + 1) % 16 == 0)
            {
                if(!curLine.empty())
                {
                    std::cout << '\t';
                    for(const char& c : curLine)
                    {
                        std::cout << c;
                    }
                    curLine.clear();
                } 
                std::cout << std::endl;
            }
        }
    }
}