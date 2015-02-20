#ifndef TOOLS_HPP
#define TOOLS_HPP

#include <string>

namespace Kelly
{
    inline bool HasContent(const char* text) noexcept
    {
        return text && *text;
    }

    std::string FileToString(const char* path);
}

#endif
