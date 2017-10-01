#ifndef TOKEN_HPP
#define TOKEN_HPP

#include "Region.hpp"
#include "View.hpp"
#include <vector>
#include <string>
#include <cstdint>

namespace Kelly
{
    struct Token
    {
        int32_t start;
        int32_t length;
        int32_t row;
        int32_t column;
        int16_t type;

        static constexpr int16_t Unknown = 0;
        static constexpr int16_t Identifier = 1;
        static constexpr int16_t Symbol = 2;
        static constexpr int16_t NumericLiteral = 3;
        static constexpr int16_t StringLiteral = 4;
    };

    const char* TokenTypeString(int16_t tokenType);

    template<typename T> struct Value
    {
        int32_t index;
        T value;
    };

    struct TreeFood
    {
        std::string path;
        std::string source;
        std::vector<Token> tokens;
        std::vector<Value<uint64_t>> integers;
        std::vector<Value<double>> floats;
        std::vector<Value<View<const char>>> strings;
        Region stringMemory;
    };

    TreeFood ParseFile(const char* path);
}

#endif
