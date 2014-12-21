#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <iostream>
#include "View.hpp"

namespace Kelly
{
    struct Token
    {
        enum class Types
        {
            None,
            Identifier,
            StringLiteral,
            NumberLiteral,
            Operator
        };

        Types type;
        View<const char> view;
    };

    Token FromSource(const char* buffer);

    std::ostream& operator<<(std::ostream& stream, const Token& token);
    std::ostream& operator<<(std::ostream& stream, Token::Types type);
}

#endif
