#include "Token.hpp"
#include "DataMap.hpp"
#include "CString.hpp"
#include <iostream>
#include <fstream>
#include <algorithm>

static DataMap<String4, int> theTokenIndicesBySymbols;
static DataMap<CString, int> theTokenIndicesByKeyword;

// Make sure these symbols are ascending order always!
static char theSymbols[] = "!#$%&()*+,-./:;<=>?@[\\]^`{|}~";
static char theEscapeSequences[] = "\"'?\\abfnrtv";

static const TokenMeta ReservedTokens[] = {
    {"return", "'return' statement", "Return"},
    {"to", "'to' operator", "To"},
    {"import", "'import' directive", "Import"},
    {"export", "'export' directive", "Export"},
    {"!", "logical not operator", "Bang"},
    {"#", "pound operator", "Pound"},
    {"$", "dollar operator", "Dollar"},
    {"%", "remainder operator", "Percent"},
    {"&", "bitwise and operator", "Ampersand"},
    {"(", "open parenthesis", "OpenParenthesis"},
    {")", "close parenthesis", "CloseParenthesis"},
    {"*", "multiplication operator", "Asterisk"},
    {"+", "addition operator", "Plus"},
    {",", "comma", "Comma"},
    {"-", "subtraction operator", "Minus"},
    {".", "dot", "Dot"},
    {"/", "division operator", "Slash"},
    {":", "colon", "Colon"},
    {";", "semicolon", "Semicolon"},
    {"<", "less than operator", "LessThan"},
    {"=", "assignment operator", "Equal"},
    {">", "greater than operator", "GreaterThan"},
    {"?", "question mark", "Question"},
    {"@", "at symbol", "At"},
    {"[", "open bracket", "OpenBracket"},
    {"\\", "backslash", "Backslash"},
    {"]", "close bracket", "CloseBracket"},
    {"^", "bitwise xor operator", "Caret"},
    {"`", "backtick", "Backtick"},
    {"{", "open brace", "OpenBrace"},
    {"|", "bitwise or operator", "Pipe"},
    {"}", "close brace", "CloseBrace"},
    {"~", "bitwise not operator", "Tilde"},
    {"&&", "logical and operator", "DoubleAmpersand"},
    {"++", "increment operator", "DoublePlus"},
    {"--", "decrement operator", "DoubleMinus"},
    {"<<", "left shift operator", "LeftShift"},
    {"!=", "inequality operator", "NotEqual"},
    {"%=", "remainder assignment operator", "ModAssign"},
    {"&=", "and assignment operator", "AndAssign"},
    {"*=", "multiplication assignment operator", "MultAssign"},
    {"+=", "addition assignment operator", "AddAssign"},
    {"-=", "subtraction assignment operator", "SubAssign"},
    {"/=", "division assignment operator", "DivAssign"},
    {"<=", "less than or equal to operator", "LessThanOrEqualTo"},
    {"==", "equality operator", "EqualTo"},
    {">=", "greater than or equal to operator", "GreaterThanOrEqualTo"},
    {"^=", "xor assignment operator", "XorAssign"},
    {"|=", "or assignment operator", "OrAssign"},
    {">>", "right shift operator", "RightShift"},
    {"||", "logical or operator", "LogicalOr"},
    {"<<=", "left shift assignment operator", "LeftShiftAssign"},
    {">>=", "right shift assignment operator", "RightShiftAssign"}};

static constexpr int ReservedTokenCount =
    sizeof(ReservedTokens) / sizeof(*ReservedTokens);

const TokenMeta* ReservedToken(int index)
{
    return ReservedTokens + index;
}

int FindOperator(String4 source)
{
    auto index = theTokenIndicesBySymbols.TryGetValue(source);
    return index ? *index : -1;
}

int FindKeyword(const char* source)
{
    auto index = theTokenIndicesByKeyword.TryGetValue({source});
    return index ? *index : -1;
}

bool IsSymbol(char c)
{
    return std::binary_search(
        theSymbols,
        theSymbols + sizeof(theSymbols) - 1,
        c);
}

bool IsEscapeSequence(char c)
{
    return std::binary_search(
        theEscapeSequences,
        theEscapeSequences + sizeof(theEscapeSequences) - 1,
        c);
}

static void MapOperatorToken(const char* source, int index)
{
    String4 ts = {};
    ts.Assign(source);
    theTokenIndicesBySymbols.Set(ts, index);
}

static void MapKeywordToken(const char* keyword, int index)
{
    CString cs = {keyword};
    theTokenIndicesByKeyword.Set(cs, index);
}

void PrepareTokens()
{
    for (int i = 0; i < ReservedTokenCount; ++i)
    {
        if (IsSymbol(ReservedTokens[i].source[0]))
            MapOperatorToken(ReservedTokens[i].source, i);
        else
            MapKeywordToken(ReservedTokens[i].source, i);
    }
    
    std::sort(theSymbols, theSymbols + sizeof(theSymbols) - 1);
    std::cout << theSymbols << '\n';
    
    std::sort(
        theEscapeSequences,
        theEscapeSequences + sizeof(theEscapeSequences) - 1);
    std::cout << theEscapeSequences << '\n';
}

void GenerateSource()
{
    std::ofstream fout("token-source.txt", std::ofstream::binary);

    auto intro = "constexpr int "; 

    for (int i = 0; i < ReservedTokenCount; ++i)
    {
        fout << intro
            << ReservedTokens[i].variable
            << " = " << i << ";\n";
    }

    fout.close();
}

const char* TokenTypeName(TokenType tokenType)
{
    switch (tokenType)
    {
        case TokenType::None: return "none";
        case TokenType::Comment: return "comment";
        case TokenType::Identifier: return "identifier";
        case TokenType::StringLiteral: return "string literal";
        case TokenType::CodePointLiteral: return "code point literal";
        case TokenType::Float32Literal: return "32-bit float literal";
        case TokenType::Float64Literal: return "64-bit float literal";
        case TokenType::SignedIntegerLiteral: return "signed integer literal";
        case TokenType::UnsignedIntegerLiteral: return "unsigned integer literal";
        case TokenType::Reserved: return "reserved";
        default: return "unknown";
    }
}

std::ostream& operator<<(std::ostream& stream, TokenType tokenType)
{    
    return stream << TokenTypeName(tokenType);
}

