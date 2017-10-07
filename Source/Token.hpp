#ifndef TokenHpp
#define TokenHpp

#include "TinyString.hpp"

namespace TokenIndex
{
    // Generated code.
    constexpr int Return = 0;
    constexpr int To = 1;
    constexpr int Import = 2;
    constexpr int Export = 3;
    constexpr int Bang = 4;
    constexpr int Pound = 5;
    constexpr int Dollar = 6;
    constexpr int Percent = 7;
    constexpr int Ampersand = 8;
    constexpr int OpenParenthesis = 9;
    constexpr int CloseParenthesis = 10;
    constexpr int Asterisk = 11;
    constexpr int Plus = 12;
    constexpr int Comma = 13;
    constexpr int Minus = 14;
    constexpr int Dot = 15;
    constexpr int Slash = 16;
    constexpr int Colon = 17;
    constexpr int Semicolon = 18;
    constexpr int LessThan = 19;
    constexpr int Equal = 20;
    constexpr int GreaterThan = 21;
    constexpr int Question = 22;
    constexpr int At = 23;
    constexpr int OpenBracket = 24;
    constexpr int Backslash = 25;
    constexpr int CloseBracket = 26;
    constexpr int Caret = 27;
    constexpr int Backtick = 28;
    constexpr int OpenBrace = 29;
    constexpr int Pipe = 30;
    constexpr int CloseBrace = 31;
    constexpr int Tilde = 32;
    constexpr int DoubleAmpersand = 33;
    constexpr int DoublePlus = 34;
    constexpr int DoubleMinus = 35;
    constexpr int LeftShift = 36;
    constexpr int NotEqual = 37;
    constexpr int ModAssign = 38;
    constexpr int AndAssign = 39;
    constexpr int MultAssign = 40;
    constexpr int AddAssign = 41;
    constexpr int SubAssign = 42;
    constexpr int DivAssign = 43;
    constexpr int LessThanOrEqualTo = 44;
    constexpr int EqualTo = 45;
    constexpr int GreaterThanOrEqualTo = 46;
    constexpr int XorAssign = 47;
    constexpr int OrAssign = 48;
    constexpr int RightShift = 49;
    constexpr int LogicalOr = 50;
    constexpr int LeftShiftAssign = 51;
    constexpr int RightShiftAssign = 52;
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
