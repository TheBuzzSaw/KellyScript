#ifndef SourceFileHpp
#define SourceFileHpp

#include <vector>
#include <string>
#include <iostream>
#include <cstdint>

struct TextPosition
{
    int line;
    int column;
};

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

struct SourceToken
{
    int offset;
    int length;
    TextPosition textPosition;
    TokenType tokenType;
    int tokenIndex;
};

union Literal
{
    float asFloat32;
    double asFloat64;
    int64_t asInt64;
    uint64_t asUInt64;
    int asCodePoint;
};

struct SourceFile
{
    std::string file;
    std::string source;
    std::vector<SourceToken> sourceTokens;
    std::vector<Literal> literals;
    std::vector<std::string> strings;
};

void PrepareLexer();
SourceFile LexSource(const char* file);
const char* TokenTypeName(TokenType tokenType);
std::ostream& operator<<(std::ostream& stream, TokenType tokenType);
std::ostream& operator<<(std::ostream& stream, TextPosition position);
std::ostream& operator<<(std::ostream& stream, const SourceFile& sourceFile);

#endif

