#include "Token.hpp"
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
        bool result = false;

        switch (input)
        {
            case '+':
            case '-':
            case '*':
            case '/':
            case '=':
            case '.':
                result = true;
                break;

            default:
                break;
        }

        return result;
    }

    Token::Token(const char* buffer)
    {
        _type = Types::None;
        _view.first = buffer;
        _view.length = 0;

        if (_view.first)
        {
            const char*& _start = _view.first;
            while (*_start && (*_start == ' ' || *_start == '\n'
                || *_start == '\r'))
            {
                ++_start;
            }

            char c = *_start; // I hate typing.
            //printf("starting -- %d -- %c\n", _start - buffer, c);

            if (IsDigit(c))
            {
                ParseNumberLiteral();
            }
            else if (c == '"')
            {
                ParseStringLiteral();
            }
            else if (c == '_' || IsLetter(c))
            {
                ParseIdentifier();
            }
            else if (IsOperator(c))
            {
                ParseOperator();
            }
        }
    }

    Token Token::Next() const
    {
        return Token(end(_view));
    }

    void Token::ParseIdentifier()
    {
        _type = Types::Identifier;

        while (IsIdentifierSafe(_view.first[++_view.length]))
            ;
    }

    void Token::ParseNumberLiteral()
    {
        _type = Types::NumberLiteral;

        bool consumedDecimal = false;

        while (true)
        {
            char c = _view.first[++_view.length];

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

    void Token::ParseStringLiteral()
    {
        _type = Types::StringLiteral;

        while (true)
        {
            char c = _view.first[++_view.length];

            if (c == '"')
            {
                ++_view.length;
                break;
            }
            else if (!c || c == '\n' || c == '\r')
            {
                _type = Types::None;
                _view.length = 0;
                break;
            }
        }
    }

    void Token::ParseOperator()
    {
        _type = Types::Operator;

        while (IsOperator(_view.first[++_view.length]))
            ;
    }

    std::ostream& operator<<(std::ostream& stream, Token::Types type)
    {
        const char* result = "unknown";

        switch (type)
        {
            case Token::Types::None: result = "none"; break;
            case Token::Types::Identifier: result = "identifier"; break;
            case Token::Types::StringLiteral: result = "string literal"; break;
            case Token::Types::NumberLiteral: result = "number literal"; break;
            case Token::Types::Operator: result = "operator"; break;

            default:
                break;
        }

        stream << result;

        return stream;
    }

    std::ostream& operator<<(std::ostream& stream, const Token& token)
    {
        const char* start = token._view.first;

        if (start)
        {
            for (std::size_t i = 0; i < token._view.length; ++i)
                stream << start[i];
        }

        return stream;
    }
}
