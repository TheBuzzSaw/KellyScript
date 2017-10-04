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
    None,
    Identifier,
    NumericLiteral,
    StringLiteral,
    CharacterLiteral,
    Symbols,
    Comment,
    
    Comma,
    OpenParen,
    CloseParen,
    OpenBracket,
    CloseBracket,
    OpenBrace,
    CloseBrace,
    AssignOp,
    AddAssignOp,
    SubAssignOp,
    MultAssignOp,
    DivAssignOp,
    ModAssignOp,
    AndAssignOp,
    OrAssignOp,
    XorAssignOp,
    LeftShiftAssignOp,
    RightShiftAssignOp,
    AddOp,
    SubOp,
    MultOp,
    DivOp,
    ModOp,
    LeftShiftOp,
    RightShiftOp,
    IncOp,
    DecOp,
    LogicalNotOp,
    BitwiseNotOp,
    LogicalAndOp,
    LogicalOrOp,
    BitwiseAndOp,
    BitwiseOrOp,
    BitwiseXorOp,
    EqualOp,
    NotEqualOp,
    LessOp,
    LessOrEqualOp,
    GreaterOp,
    GreaterOrEqualOp,
    
    Dot,
    Semicolon,
    Colon,
    Question,
    
    TokenCount
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

const char* TokenName(Token token);
void PrepareLexer();
SourceFile LexSource(const char* file);
std::ostream& operator<<(std::ostream& stream, Token token);
std::ostream& operator<<(std::ostream& stream, TextPosition position);
std::ostream& operator<<(std::ostream& stream, const SourceFile& sourceFile);

#endif

