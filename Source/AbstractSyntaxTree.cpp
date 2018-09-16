#include "AbstractSyntaxTree.hpp"

struct Parser
{
    const SourceFile* sourceFile = nullptr;
    View<const SourceToken> sourceTokens = {};
    Region* region = nullptr;
    int index = 0;
    std::string errorMessage;
    TextPosition errorPosition;

    bool HasError()
    {
        return !errorMessage.empty();
    }

    SourceToken Current()
    {
        return sourceTokens[index];
    }

    bool InRange()
    {
        return index < sourceTokens.count;
    }

    bool AcceptReserved(int reservedIndex)
    {
        if (Current().IsReserved(reservedIndex))
        {
            ++index;
            return true;
        }

        return false;
    }

    void ExpectReserved(int reservedIndex)
    {
        
    }

    AbstractNode* ExpectIdentifier()
    {
        if (Current().tokenType != TokenType::Identifier)
        {
            errorMessage = "expected identifier";
            return nullptr;
        }

        auto tree = new (*region) AbstractNode;

        return tree;
    }
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

    while (!parser.HasError() && parser.AcceptReserved(TokenIndex::Import))
    {
        auto tree = parser.ExpectIdentifier();
        if (!tree) break;
        ast.imports.push_back(tree);
    }


    return ast;
}
