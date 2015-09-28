#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <iostream>
#include <vector>

namespace Kelly
{
    struct Token
    {
        enum class Type : int
        {
            None,
            Identifier,
            StringLiteral,
            NumericLiteral,
            SymbolString,
            InvalidCodePoint
        };

        Type type;
        int offset;
        int length;
        int row;
        int column;
    };

    std::vector<Token> GetTokens(const char* buffer);

    std::ostream& operator<<(std::ostream& stream, const Token& token);
    std::ostream& operator<<(std::ostream& stream, Token::Type type);
}

#endif
