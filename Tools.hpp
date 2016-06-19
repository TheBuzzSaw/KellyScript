#ifndef TOOLS_HPP
#define TOOLS_HPP

#include <string>

namespace Kelly
{
    template<typename T> void Write(void* data, T value)
    {
        *reinterpret_cast<T*>(data) = value;
    }

    template<typename T> T Read(const void* data)
    {
        return *reinterpret_cast<const T*>(data);
    }

    inline bool HasContent(const char* text) noexcept
    {
        return text && *text;
    }

    std::string FileToString(const char* path);
}

#endif
