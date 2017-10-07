#include "Token.hpp"
#include "DataMap.hpp"
#include "CString.hpp"
#include <iostream>
#include <algorithm>

static DataMap<String4, int> theTokenIndicesBySymbols;
static DataMap<CString, int> theTokenIndicesByKeyword;

// Make sure these symbols are ascending order always!
static char theSymbols[] = "!#$%&()*+,-./:;<=>?@[\\]^`{|}~";
static char theEscapeSequences[] = "\"'?\\abfnrtv";

static const TokenMeta KeywordTokens[] = {
    {"return", "return statement"},
    {"to", "to statement"}};

static constexpr int KeywordTokenCount =
    sizeof(KeywordTokens) / sizeof(*KeywordTokens);

static const TokenMeta OperatorTokens[] = {
    {"!", "logical not operator"},
    {"#", "pound operator"},
    {"$", "dollar operator"},
    {"%", "remainder operator"},
    {"&", "bitwise and operator"},
    {"(", "open parenthesis"},
    {")", "close parenthesis"},
    {"*", "multiplication operator"},
    {"+", "addition operator"},
    {",", "comma"},
    {"-", "subtraction operator"},
    {".", "dot"},
    {"/", "division operator"},
    {":", "colon"},
    {";", "semicolon"},
    {"<", "less than operator"},
    {"=", "assignment operator"},
    {">", "greater than operator"},
    {"?", "question mark"},
    {"@", "at symbol"},
    {"[", "open bracket"},
    {"\\", "backslash"},
    {"]", "close bracket"},
    {"^", "bitwise xor operator"},
    {"`", "backtick"},
    {"{", "open brace"},
    {"|", "bitwise or operator"},
    {"}", "close brace"},
    {"~", "bitwise not operator"},
    {"&&", "logical and operator"},
    {"++", "increment operator"},
    {"--", "decrement operator"},
    {"<<", "left shift operator"},
    {"!=", "inequality operator"},
    {"%=", "remainder assignment operator"},
    {"&=", "and assignment operator"},
    {"*=", "multiplication assignment operator"},
    {"+=", "addition assignment operator"},
    {"-=", "subtraction assignment operator"},
    {"/=", "division assignment operator"},
    {"<=", "less than or equal to operator"},
    {"==", "equality operator"},
    {">=", "greater than or equal to operator"},
    {"^=", "xor assignment operator"},
    {"|=", "or assignment operator"},
    {">>", "right shift operator"},
    {"||", "logical or operator"},
    {"<<=", "left shift assignment operator"},
    {">>=", "right shift assignment operator"}};

static constexpr int OperatorTokenCount =
    sizeof(OperatorTokens) / sizeof(*OperatorTokens);

const TokenMeta* KeywordToken(int index)
{
    return KeywordTokens + index;
}

const TokenMeta* OperatorToken(int index)
{
    return OperatorTokens + index;
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
    for (int i = 0; i < OperatorTokenCount; ++i)
        MapOperatorToken(OperatorTokens[i].source, i);
    
    for (int i = 0; i < KeywordTokenCount; ++i)
        MapKeywordToken(KeywordTokens[i].source, i);
    
    std::sort(theSymbols, theSymbols + sizeof(theSymbols) - 1);
    std::cout << theSymbols << '\n';
    
    std::sort(
        theEscapeSequences,
        theEscapeSequences + sizeof(theEscapeSequences) - 1);
    std::cout << theEscapeSequences << '\n';
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
        case TokenType::Keyword: return "keyword";
        case TokenType::Operator: return "operator";
        default: return "unknown";
    }
}

std::ostream& operator<<(std::ostream& stream, TokenType tokenType)
{    
    return stream << TokenTypeName(tokenType);
}

