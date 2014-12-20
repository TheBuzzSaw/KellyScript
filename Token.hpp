#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <iostream>
#include "View.hpp"

namespace Kelly
{
    class Token
    {
        public:
            enum class Types { None, Identifier, StringLiteral,
                NumberLiteral, Operator };

            Token(const char* buffer);
            Token(const Token& other) = default;
            ~Token() = default;

            Token& operator=(const Token& other) = default;


            inline Types Type() const { return _type; }

            Token Next() const;

            friend std::ostream& operator<<(
                std::ostream& stream,
                const Token& token);

        protected:
        private:
            void ParseIdentifier();
            void ParseNumberLiteral();
            void ParseStringLiteral();
            void ParseOperator();

            Types _type;
            View<const char> _view;
    };

    std::ostream& operator<<(std::ostream& stream, Token::Types type);
}

#endif
