#ifndef TOOLS_HPP
#define TOOLS_HPP

#include <vector>

namespace Kelly
{
    inline bool HasContent(const char* text)
    {
        return text && *text;
    }

    std::vector<char> FileToString(const char* path);
}

#endif
