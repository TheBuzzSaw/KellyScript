#ifndef Utf8CodePoint_HPP
#define Utf8CodePoint_HPP

#include "View.hpp"

namespace Kelly
{
    /// http://en.wikipedia.org/wiki/UTF-8

    union Utf8CodePoint
    {
        uint32_t value;
        uint8_t bytes[4];
        char chars[4];
    };

    constexpr bool operator==(const Utf8CodePoint& a, const Utf8CodePoint& b)
    {
        return a.value == b.value;
    }

    constexpr bool operator!=(const Utf8CodePoint& a, const Utf8CodePoint& b)
    {
        return a.value != b.value;
    }

    constexpr const View<char> GetView(Utf8CodePoint& codePoint)
    {
        return { reinterpret_cast<char*>(codePoint.chars), 4 };
    }

    constexpr const View<const char> GetConstView(
        const Utf8CodePoint& codePoint)
    {
        return { reinterpret_cast<const char*>(codePoint.chars), 4 };
    }

    const Utf8CodePoint GetUtf8CodePoint(const char* text);
    size_t GetLength(const Utf8CodePoint& codePoint);
    int32_t GetUtf32CodePoint(const Utf8CodePoint& codePoint);
}

#endif
