#include "Token.hpp"
#include "Tools.hpp"
#include <fstream>
#include <algorithm>
#include <cstdio>
#include <cassert>

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

    TreeFood ParseFile(const char* path)
    {
        TreeFood result;

        result.source = FileToString(path);

        int32_t row = 1;
        int32_t column = 1;
        std::string buffer;

        auto source = result.source.data();
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

            Token token;
            token.start = i;
            token.length = 1;
            token.row = row;
            token.column = column;
            token.type = Token::Unknown;

            if (IsDigit(c))
            {
                token.type = Token::NumericLiteral;

                while (IsDigit(source[i + 1]))
                {
                    ++i;
                    ++column;
                    ++token.length;
                }
            }
            else if (c == '_' || IsLetter(c))
            {
                token.type = Token::Identifier;

                while (IsIdentifierSafe(source[i + 1]))
                {
                    ++i;
                    ++column;
                    ++token.length;
                }
            }
            else if (c == '"')
            {
                token.type = Token::StringLiteral;
                buffer.clear();

                int skip = 0;

                for (auto j = i + 1; source[j]; ++j)
                {
                    ++i;
                    ++column;
                    ++token.length;

                    if (skip > 0)
                    {
                        --skip;
                        continue;
                    }

                    if (source[j] == '"') break;

                    if (source[j] == '\\')
                    {
                        skip = 1;
                        switch(source[j + 1])
                        {
                            case '"': buffer += '"'; break;
                            case 'n': buffer += '\n'; break;
                            case 't': buffer += '\t'; break;
                            case '\\': buffer += '\\'; break;
                            default: assert(false); break;
                        }
                    }
                    else if (IsStringLiteralSafe(source[j]))
                    {
                        buffer += source[j];
                    }
                    else
                    {
                        assert(false);
                    }
                }
            }
            else if (IsSymbol(c))
            {
                token.type = Token::Symbol;

                while (IsSymbol(source[i + 1]))
                {
                    ++i;
                    ++column;
                    ++token.length;
                }
            }

            if (token.type != Token::Unknown) result.tokens.push_back(token);
        }

        return result;
    }
}
