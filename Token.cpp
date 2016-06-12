#include "Token.hpp"
#include <algorithm>
#include <cstdio>

namespace Kelly
{
    static const char Symbols[] = "!#$%&()*+,-./:;<=>?@[\\]^_`{|}~";

    template<typename T> constexpr bool IsBetween(T n, T first, T last)
    {
        return first <= n && n <= last;
    }

    template<typename T> constexpr bool IsLowercase(T n)
    {
        return IsBetween(n, T('a'), T('z'));
    }

    template<typename T> constexpr bool IsUppercase(T n)
    {
        return IsBetween(n, T('A'), T('Z'));
    }

    template<typename T> constexpr bool IsLetter(T n)
    {
        return IsLowercase(n) || IsUppercase(n);
    }

    template<typename T> constexpr bool IsDigit(T n)
    {
        return IsBetween(n, T('0'), T('9'));
    }

    template<typename T> constexpr bool IsIdentifierSafe(T n)
    {
        return n == '_' ||
            IsLetter(n) ||
            IsDigit(n);
    }

    template<typename T> constexpr bool IsStringLiteralSafe(T n)
    {
        return n && n != '"' && n != '\n';
    }

    bool IsSymbol(char input)
    {
        return std::binary_search(
            Symbols, Symbols + sizeof(Symbols) - 1, input);
    }

    std::vector<Token> GetTokens(const char* source)
    {
        std::vector<Token> tokens;

        int32_t row = 1;
        int32_t column = 1;

        for (int32_t i = 0; source[i]; ++i, ++column)
        {
            int c = source[i];

            if (c == '\n')
            {
                ++row;
                column = 0;
                continue;
            }

            if (c == ' ' || c == '\r') continue;

            if (IsDigit(c))
            {
                Token token;
                token.start = i;
                token.length = 1;
                token.row = row;
                token.column = column;
                token.type = Token::NumericLiteral;

                while (IsDigit(source[i + 1]))
                {
                    ++i;
                    ++column;
                    ++token.length;
                }

                tokens.push_back(token);
            }
            else if (c == '_' || IsLetter(c))
            {
                Token token;
                token.start = i;
                token.length = 1;
                token.row = row;
                token.column = column;
                token.type = Token::Identifier;

                while (IsIdentifierSafe(source[i + 1]))
                {
                    ++i;
                    ++column;
                    ++token.length;
                }

                tokens.push_back(token);
            }
            else if (c == '"')
            {
                Token token;
                token.start = i;
                token.length = 1;
                token.row = row;
                token.column = column;
                token.type = Token::StringLiteral;

                while (IsStringLiteralSafe(source[i + 1]))
                {
                    ++i;
                    ++column;
                    ++token.length;
                }

                if (source[i + 1] == '"')
                {
                    ++i;
                    ++column;
                    ++token.length;
                }

                tokens.push_back(token);
            }
            else if (IsSymbol(c))
            {
                Token token;
                token.start = i;
                token.length = 1;
                token.row = row;
                token.column = column;
                token.type = Token::Symbol;

                tokens.push_back(token);
            }
        }

        return tokens;
    }
}
