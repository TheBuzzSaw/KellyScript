#include "Token.hpp"
#include "Utf8CodePoint.hpp"
#include <algorithm>
#include <cstdio>

namespace Kelly
{
    template<typename T> constexpr bool InRange(T value, T first, T last)
    {
        return first <= value && value <= last;
    }

    constexpr bool IsLowercase(int input)
    {
        return InRange<int>(input, 'a', 'z');
    }

    constexpr bool IsUppercase(int input)
    {
        return InRange<int>(input, 'A', 'Z');
    }

    constexpr bool IsLetter(int input)
    {
        return IsLowercase(input) || IsUppercase(input);
    }

    constexpr bool IsDigit(int input)
    {
        return InRange<int>(input, '0', '9');
    }

    constexpr bool IsIdentifierSafe(int input)
    {
        return IsLetter(input) || IsDigit(input) || input == '_';
    }

    bool IsSymbol(int input)
    {
        const char Symbols[] = "!#$%&()*+,-./:;<=>?@[\\]^_`{|}~";
        const auto LastSymbol = Symbols + sizeof(Symbols) - 1;

        return InRange<int>(input, *Symbols, *LastSymbol) &&
            std::binary_search(Symbols, LastSymbol, input);
    }

    void ParseIdentifier(Token& token)
    {
        token.type = Token::Type::Identifier;

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

    std::vector<Token> GetTokens(const char* buffer)
    {
        std::vector<Token> result;

        int row = 1;
        int column = 0;
        int offset = 0;

        for (auto i = buffer; *i; ++i)
        {
            Token token;
            token.type = Token::Type::None;
            token.offset = offset;
            token.row = row;
            token.column = ++column;

            Utf8ParseResult parsedCodePoint = ParseUtf8CodePoint(i);

            if (parsedCodePoint.parseResult < 0)
            {
                token.type = Token::Type::InvalidCodePoint;
                result.push_back(token);
                return result;
            }

            offset += parsedCodePoint.parseResult;
            int codePoint = GetUtf32CodePoint(parsedCodePoint.codePoint);


        }

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
