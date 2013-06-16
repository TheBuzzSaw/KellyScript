#ifndef TOKEN_HPP
#define TOKEN_HPP

#include "Primitives.hpp"
#include <iostream>

namespace Kelly
{
    class Token
    {
        public:
            enum class Types { None, Identifier, StringLiteral, NumberLiteral,
                Operator };

            Token(const char* buffer);
            Token(const Token& other);
            Token(Token&& other);
            ~Token();

            Token& operator=(const Token& other);

            inline Size Length() const { return _length; }
            inline const char* Start() const { return _start; }
            inline Types Type() const { return _type; }

        protected:
        private:
            void ParseIdentifier();
            void ParseNumberLiteral();
            void ParseStringLiteral();
            void ParseOperator();

            Types _type;
            const char* _start;
            Size _length;
    };

    std::ostream& operator<<(std::ostream& stream, Token::Types type);
    std::ostream& operator<<(std::ostream& stream, const Token& token);
}

#endif
