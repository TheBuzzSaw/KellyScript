#ifndef AbstractSyntaxTreeHpp
#define AbstractSyntaxTreeHpp

#include "SourceFile.hpp"
#include "Region.hpp"
#include <cstdint>

struct AbstractNode
{
    uint32_t typeId;
};

struct BinaryExpression : AbstractNode
{
    AbstractNode* left;
    AbstractNode* right;
};

struct ValueNode : AbstractNode
{
    int index;
};

struct IdentifierNode : AbstractNode
{
    int offset;
    int length;
};

struct AbstractSyntaxTree
{
    std::vector<AbstractNode*> imports;
};

AbstractSyntaxTree Parse(const SourceFile& sourceFile, Region& region);

#endif
