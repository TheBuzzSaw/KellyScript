#include "Token.hpp"
#include <algorithm>
#include <cstdio>

namespace Kelly
{
    constexpr bool IsBetween(char input, char first, char last)
    {
        return first <= input && input <= last;
    }

    constexpr bool IsLowercase(char input)
    {
        return IsBetween(input, 'a', 'z');
    }

    constexpr bool IsUppercase(char input)
    {
        return IsBetween(input, 'A', 'Z');
    }

    constexpr bool IsLetter(char input)
    {
        return IsLowercase(input) || IsUppercase(input);
    }

    constexpr bool IsDigit(char input)
    {
        return IsBetween(input, '0', '9');
    }

    constexpr bool IsIdentifierSafe(char input)
    {
        return IsLetter(input) || IsDigit(input) || input == '_';
    }

    bool IsOperator(char input)
    {
        const char Symbols[] = "!#$%&()*+,-./:;<=>?@[\\]^_`{|}~";
        return std::binary_search(
            Symbols, Symbols + sizeof(Symbols) - 1, input);
    }

    void ParseIdentifier(Token& token)
    {
        token.type = Token::Types::Identifier;

        while (IsIdentifierSafe(token.source.first[++token.source.length]))
            ;
    }

    void ParseNumberLiteral(Token& token)
    {
        token.type = Token::Types::NumberLiteral;

        bool consumedDecimal = false;

        while (true)
        {
            char c = token.source.first[++token.source.length];

            if (c == '.')
            {
                if (consumedDecimal) break;

                consumedDecimal = true;
            }
            else if (!IsDigit(c))
            {
                break;
            }
        }
    }

    void ParseStringLiteral(Token& token)
    {
        token.type = Token::Types::StringLiteral;

        while (true)
        {
            char c = token.source.first[++token.source.length];

            if (c == '"')
            {
                ++token.source.length;
                break;
            }
            else if (!c || c == '\n' || c == '\r')
            {
                token.type = Token::Types::None;
                token.source.length = 0;
                break;
            }
        }
    }

    void ParseOperator(Token& token)
    {
        token.type = Token::Types::Operator;

        while (IsOperator(token.source.first[++token.source.length]))
            ;
    }

    Token FromSource(const char* buffer)
    {
        Token result;
        result.type = Token::Types::None;
        result.source.first = buffer;
        result.source.length = 0;

        if (result.source.first)
        {
            const char*& i = result.source.first;
            while (*i && (*i == ' ' || *i == '\n' || *i == '\r')) ++i;

            char c = *i;

            if (IsDigit(c))
            {
                ParseNumberLiteral(result);
            }
            else if (c == '"')
            {
                ParseStringLiteral(result);
            }
            else if (c == '_' || IsLetter(c))
            {
                ParseIdentifier(result);
            }
            else if (IsOperator(c))
            {
                ParseOperator(result);
            }
        }

        return result;
    }

    std::ostream& operator<<(std::ostream& stream, Token::Types type)
    {
        const char* result = "unknown";

        switch (type)
        {
            case Token::Types::None: result = "none"; break;
            case Token::Types::Space: result = "space"; break;
            case Token::Types::Identifier: result = "identifier"; break;
            case Token::Types::StringLiteral: result = "string literal"; break;
            case Token::Types::NumberLiteral: result = "number literal"; break;
            case Token::Types::Operator: result = "operator"; break;

            default:
                break;
        }

        return stream << result;
    }

    std::ostream& operator<<(std::ostream& stream, const Token& token)
    {
        return stream << token.type << ": " << token.source;
    }
}
