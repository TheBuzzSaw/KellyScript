#include "AbstractSyntaxTree.hpp"

struct Parser
{
    const SourceFile* sourceFile;
    Region* region;
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
