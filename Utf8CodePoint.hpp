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

    struct Utf8ParseResult
    {
        static constexpr int InvalidByteCount = -1;
        static constexpr int MissingContinuationByte = -2;
        static constexpr int NullPointer = -3;

        Utf8CodePoint codePoint;
        int parseResult;
    };

    constexpr bool operator==(Utf8CodePoint a, Utf8CodePoint b)
    {
        return a.value == b.value;
    }

    constexpr bool operator!=(Utf8CodePoint a, Utf8CodePoint b)
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

    Utf8CodePoint GetUtf8CodePoint(const char* text);
    size_t GetLength(Utf8CodePoint codePoint);
    int32_t GetUtf32CodePoint(Utf8CodePoint codePoint);
}

#endif
