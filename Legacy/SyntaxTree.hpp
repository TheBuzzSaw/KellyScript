#ifndef SYNTAXTREE_HPP
#define SYNTAXTREE_HPP

#include "Token.hpp"

namespace Kelly
{
    struct Package
    {
        View<const char> name;
        int32_t parentIndex;
    };

    struct AbstractDataType
    {
        View<const char> name;
        int32_t packageIndex;
    };

    struct ConcreteDataType
    {
        View<const char> name;
        int32_t packageIndex;
        int32_t byteCount;
    };

    struct SubTree
    {
        View<const char> exportedPackageName;
        std::vector<View<const char>> importedPackageNames;
        // define new structs
        // define new functions
        // define new globals
    };

    struct AbstractSyntaxTree
    {
        Region nodeMemory;
        Region stringMemory;
        std::vector<AbstractDataType> dataTypes;
        std::vector<SubTree> subTrees;

        void Eat(const TreeFood& food);
    };

    struct ConcreteSyntaxTree
    {
        Region nodeMemory;
        Region stringMemory;
        std::vector<Package> packages;
        std::vector<ConcreteDataType> dataTypes;
    };
}

#endif
