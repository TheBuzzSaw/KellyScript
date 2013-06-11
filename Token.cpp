#include "Token.hpp"
#include <cstdio>

namespace Kelly
{
    namespace
    {
        inline bool IsBetween(char input, char first, char last)
        {
            return input >= first && input <= last;
        }

        inline bool IsLowercase(char input)
        {
            return IsBetween(input, 'a', 'z');
        }

        inline bool IsUppercase(char input)
        {
            return IsBetween(input, 'A', 'Z');
        }

        inline bool IsLetter(char input)
        {
            return IsLowercase(input) || IsUppercase(input);
        }

        inline bool IsDigit(char input)
        {
            return IsBetween(input, '0', '9');
        }

        inline bool IsIdentifierSafe(char input)
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
    }

    Token::Token(const char* buffer)
    {
        _type = None;
        _start = buffer;
        _length = 0;

        if (_start)
        {
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

    Token::Token(const Token& other)
    {
        _type = other._type;
        _start = other._start;
        _length = other._length;
    }

    Token::~Token()
    {
    }

    Token& Token::operator=(const Token& other)
    {
        _type = other._type;
        _start = other._start;
        _length = other._length;
        return *this;
    }

    void Token::ParseIdentifier()
    {
        _type = Identifier;

        while (IsIdentifierSafe(_start[++_length]))
            ;
    }

    void Token::ParseNumberLiteral()
    {
        _type = NumberLiteral;

        bool consumedDecimal = false;

        while (true)
        {
            char c = _start[++_length];

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
        _type = StringLiteral;

        while (true)
        {
            char c = _start[++_length];

            if (c == '"')
            {
                ++_length;
                break;
            }
            else if (!c || c == '\n' || c == '\r')
            {
                _type = None;
                _length = 0;
                break;
            }
        }
    }

    void Token::ParseOperator()
    {
        _type = Operator;

        while (IsOperator(_start[++_length]))
            ;
    }

    std::ostream& operator<<(std::ostream& stream, Token::Type type)
    {
        const char* result = "unknown";

        switch (type)
        {
            case Token::None: result = "none"; break;
            case Token::Identifier: result = "identifier"; break;
            case Token::StringLiteral: result = "string literal"; break;
            case Token::NumberLiteral: result = "number literal"; break;
            case Token::Operator: result = "operator"; break;

            default:
                break;
        }

        stream << result;

        return stream;
    }

    std::ostream& operator<<(std::ostream& stream, const Token& token)
    {
        const char* start = token.Start();

        if (start)
        {
            for (Size i = 0; i < token.Length(); ++i)
                stream << start[i];
        }

        return stream;
    }
}
