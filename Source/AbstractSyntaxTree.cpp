#include "AbstractSyntaxTree.hpp"

struct Parser
{
    const SourceFile* sourceFile = nullptr;
    Region* region = nullptr;
    int index = 0;
    std::string errorMessage;
    TextPosition errorPosition;
};

AbstractSyntaxTree Parse(const SourceFile& sourceFile, Region& region)
{
    AbstractSyntaxTree ast;
    Parser parser;
    parser.sourceFile = &sourceFile;
    parser.region = &region;
    (void)parser;

    return ast;
}
