#ifndef SourceFileHpp
#define SourceFileHpp

#include <vector>
#include <string>

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
    Symbols
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

#endif

