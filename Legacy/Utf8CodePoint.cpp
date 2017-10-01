#include "Utf8CodePoint.hpp"

namespace Kelly
{
    // There is a 99% chance that there is better way to implement this.
    static int CountLeftBits(uint8_t n)
    {
        int result = 0;

        for (int i = 0; i < 8; ++i)
        {
            uint8_t mask = 1 << (7 - i);

            if (n & mask)
                ++result;
            else
                break;
        }

        return result;
    }

    static constexpr bool IsContinuationByte(uint8_t n)
    {
        return (n & 0xc0) == 0x80;
    }

    Utf8ParseResult ParseUtf8CodePoint(const char* text)
    {
        auto bytes = (const uint8_t*)text;
        Utf8ParseResult result{};

        if (!text)
        {
            result.parseResult = Utf8ParseResult::NullPointer;
            return result;
        }

        auto bitCount = CountLeftBits(bytes[0]);

        if (bitCount == 0)
        {
            result.codePoint.bytes[0] = bytes[0];
            result.parseResult = 1;
        }
        else if (2 <= bitCount && bitCount <= 4)
        {
            for (int i = 1; i < bitCount; ++i)
            {
                result.codePoint.bytes[i] = bytes[i];

                if (!IsContinuationByte(bytes[i]))
                {
                    result.parseResult =
                        Utf8ParseResult::MissingContinuationByte;

                    return result;
                }
            }

            result.parseResult = bitCount;
        }
        else
        {
            result.parseResult = Utf8ParseResult::InvalidByteCount;
        }

        return result;
    }

    Utf8CodePoint GetUtf8CodePoint(const char* text)
    {
        const uint8_t* bytes = reinterpret_cast<const uint8_t*>(text);
        Utf8CodePoint result = {};

        if (!text) return result;

        auto bitCount = CountLeftBits(bytes[0]);

        if (bitCount == 0)
        {
            result.bytes[0] = bytes[0];
        }
        else if (2 <= bitCount && bitCount <= 4)
        {
            for (int i = 1; i < bitCount; ++i)
            {
                if (!IsContinuationByte(bytes[i])) return result;
            }

            for (int i = 0; i < bitCount; ++i) result.bytes[i] = bytes[i];
        }

        return result;
    }

    size_t GetLength(Utf8CodePoint codePoint)
    {
        size_t result = 0;

        while (result < 4 && codePoint.bytes[result]) ++result;

        return result;
    }

    int32_t GetUtf32CodePoint(Utf8CodePoint codePoint)
    {
        auto length = GetLength(codePoint);

        if (length < 2) return codePoint.bytes[0];

        int32_t mask = ~(0xffffff00 >> length);
        int32_t result = int(codePoint.bytes[0]) & mask;

        for (size_t i = 1; i < length; ++i)
            result = (result << 6) | (codePoint.bytes[i] & 0x3f);

        return result;
    }
}
