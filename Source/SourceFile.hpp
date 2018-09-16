#ifndef SourceFileHpp
#define SourceFileHpp

#include "Token.hpp"
#include <vector>
#include <string>
#include <iostream>
#include <cstdint>

struct TextPosition
{
    int line;
    int column;
};

struct SourceToken
{
    int offset;
    int length;
    TextPosition textPosition;
    TokenType tokenType;
    int tokenIndex;

    bool IsReserved(int reservedIndex) const
    {
        return
            tokenType == TokenType::Reserved &&
            tokenIndex == reservedIndex;
    }
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

SourceFile LexSource(const char* file);
std::ostream& operator<<(std::ostream& stream, TextPosition position);
std::ostream& operator<<(std::ostream& stream, const SourceFile& sourceFile);

#endif

