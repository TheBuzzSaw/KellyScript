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
            Space,
            Identifier,
            StringLiteral,
            NumberLiteral,
            Operator
        };

        Types type = Types::None;
        View<const char> source = { nullptr, 0 };
        const char* file = nullptr;
        int row = 0;
        int column = 0;
    };

    Token FromSource(const char* buffer);

    std::ostream& operator<<(std::ostream& stream, const Token& token);
    std::ostream& operator<<(std::ostream& stream, Token::Types type);
}

#endif
