#ifndef AbstractSyntaxTreeHpp
#define AbstractSyntaxTreeHpp

#include "SourceFile.hpp"
#include "Region.hpp"

struct AbstractSyntaxTree
{
    std::vector<void*> nodes;
    std::string errorMessage;
    TextPosition errorPosition;
};

AbstractSyntaxTree Parse(const SourceFile& sourceFile, Region& region);

#endif
