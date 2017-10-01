#ifndef SourceFileHpp
#define SourceFileHpp

#include <vector>
#include <string>
#include <iostream>

struct TextPosition
{
    int line;
    int column;
};

enum class Token
{
    Identifier,
    NumericLiteral,
    StringLiteral,
    Symbols,
    Comment
};

struct SourceToken
{
    int offset;
    int length;
    TextPosition textPosition;
    Token token;
};

struct SourceFile
{
    std::string file;
    std::string source;
    std::vector<SourceToken> sourceTokens;
};

SourceFile LexSource(const char* file);
std::ostream& operator<<(std::ostream& stream, Token token);
std::ostream& operator<<(std::ostream& stream, const SourceFile& sourceFile);

#endif

