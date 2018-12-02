#include "PrintExample.h"

#include <iostream>
#include <vector>

#include <iomanip>

namespace HexReader
{
static char GetPrintChar(char c) noexcept
{
    if(('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z'))
        return c;
    return '.';
}

void PrintExample() noexcept
{
    std::vector<char> curLine;
    for(int i = 0; i < 512; i++)
    {
        std::cout << std::hex << std::uppercase << std::setw(2) << std::setfill('0') << i % 256 << ' ';
        curLine.push_back(GetPrintChar(static_cast<char>(i)));
        if((i + 1) % 16 == 0)
        {
            if(!curLine.empty())
            {
                std::cout << '\t';
                for(const char& c : curLine )
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
