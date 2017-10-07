#ifndef TokenHpp
#define TokenHpp

#include "TinyString.hpp"

namespace TokenIndex
{
    constexpr int Bang = 0;
    constexpr int Pound = 1;
    constexpr int Dollar = 2;
    constexpr int Percent = 3;
    constexpr int Ampersand = 4;
    constexpr int OpenParenthesis = 5;
    constexpr int CloseParenthesis = 6;
    constexpr int Asterisk = 7;
    constexpr int Plus = 8;
    constexpr int Comma = 9;
    constexpr int Minus = 10;
    constexpr int Dot = 11;
    constexpr int Slash = 12;
    constexpr int Colon = 13;
    constexpr int Semicolon = 14;
    constexpr int LessThan = 15;
    constexpr int Equal = 16;
    constexpr int GreaterThan = 17;
    constexpr int Question = 18;
    constexpr int At = 19;
    constexpr int OpenBracket = 20;
    constexpr int Backslash = 21;
    constexpr int CloseBracket = 22;
    constexpr int Caret = 23;
    constexpr int Backtick = 24;
    constexpr int OpenBrace = 25;
    constexpr int Pipe = 26;
    constexpr int CloseBrace = 27;
    constexpr int Tilde = 28;
    constexpr int DoubleAmpersand = 29;
    constexpr int DoublePlus = 30;
    constexpr int DoubleMinus = 31;
    constexpr int LeftShift = 32;
    constexpr int NotEqual = 33;
    constexpr int ModAssign = 34;
    constexpr int AndAssign = 35;
    constexpr int MultAssign = 36;
    constexpr int AddAssign = 37;
    constexpr int SubAssign = 38;
    constexpr int DivAssign = 39;
    constexpr int LessThanOrEqualTo = 40;
    constexpr int EqualTo = 41;
    constexpr int GreaterThanOrEqualTo = 42;
    constexpr int XorAssign = 43;
    constexpr int OrAssign = 44;
    constexpr int RightShift = 45;
    constexpr int LogicalOr = 46;
    constexpr int LeftShiftAssign = 47;
    constexpr int RightShiftAssign = 48;
}

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
    Reserved
};

struct TokenMeta
{
    const char* source;
    const char* description;
    const char* variable;
};

const TokenMeta* ReservedToken(int index);

void PrepareTokens();
void GenerateSource();
int FindOperator(String4 source);
int FindKeyword(const char* source);
bool IsSymbol(char c);
bool IsEscapeSequence(char c);

const char* TokenTypeName(TokenType tokenType);
std::ostream& operator<<(std::ostream& stream, TokenType tokenType);

constexpr bool IsSpace(char c)
{
    return
        c == ' ' ||
        c == '\r' ||
        c == '\t' ||
        c == '\n';
}

constexpr bool IsDigit(char c)
{
    return '0' <= c && c <= '9';
}

constexpr bool IsUpperHex(char c)
{
    return 'A' <= c && c <= 'F';
}

constexpr bool IsLowerHex(char c)
{
    return 'a' <= c && c <= 'f';
}

constexpr bool IsUpper(char c)
{
    return 'A' <= c && c <= 'Z';
}

constexpr bool IsLower(char c)
{
    return 'a' <= c && c <= 'z';
}

constexpr bool IsAlpha(char c)
{
    return IsUpper(c) || IsLower(c);
}

constexpr bool IsIdentifierSafe(char c)
{
    return IsAlpha(c) || IsDigit(c) || c == '_';
}

inline bool IsStringLiteralSafe(char c)
{
    return IsIdentifierSafe(c) || IsSymbol(c) || c == ' ' || c == '\'';
}

#endif
