#ifndef AbstractSyntaxTreeHpp
#define AbstractSyntaxTreeHpp

#include "SourceFile.hpp"
#include "Region.hpp"

struct AbstractSyntaxTree
{
    std::vector<void*> nodes;

    
};

AbstractSyntaxTree Parse(const SourceFile& sourceFile, Region& region);

#endif
