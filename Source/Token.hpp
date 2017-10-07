#ifndef TokenHpp
#define TokenHpp

#include "TinyString.hpp"

enum class TokenType
{
    None,
    Comment,
    Identifier,
    StringLiteral,
    CodePointLiteral,
    Float32Literal,
    Float64Literal,
    SignedIntegerLiteral,
    UnsignedIntegerLiteral,
    Keyword,
    Operator
};

struct TokenMeta
{
    const char* source;
    const char* description;
};

const TokenMeta* KeywordToken(int index);
const TokenMeta* OperatorToken(int index);

void PrepareTokens();
int FindOperator(String4 source);
int FindKeyword(const char* source);
bool IsSymbol(char c);
bool IsEscapeSequence(char c);

const char* TokenTypeName(TokenType tokenType);
std::ostream& operator<<(std::ostream& stream, TokenType tokenType);

#endif
