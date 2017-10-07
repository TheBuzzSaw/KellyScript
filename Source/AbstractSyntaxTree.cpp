#include "AbstractSyntaxTree.hpp"

struct Parser
{
    const SourceFile* sourceFile = nullptr;
    View<const SourceToken> sourceTokens = {};
    SourceToken currentToken;
    Region* region = nullptr;
    int index = -1;
    std::string errorMessage;
    TextPosition errorPosition;

    void Advance()
    {
        currentToken = sourceTokens[++index];
    }

    bool AcceptReserved(int index);
};

AbstractSyntaxTree Parse(const SourceFile& sourceFile, Region& region)
{
    AbstractSyntaxTree ast;
    Parser parser;
    parser.sourceFile = &sourceFile;
    parser.sourceTokens = {
        sourceFile.sourceTokens.data(),
        (int)sourceFile.sourceTokens.size()};
    parser.region = &region;
    (void)parser;



    return ast;
}
