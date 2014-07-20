#include "SyntaxTree.hpp"

namespace Kelly
{
    SyntaxTree::SyntaxTree()
    {
        Add(TypeDefinition("Int8", 1));
        Add(TypeDefinition("Int16", 2));
        Add(TypeDefinition("Int32", 4));
        Add(TypeDefinition("Int64", 8));
    }

    SyntaxTree::SyntaxTree(SyntaxTree&& other)
        : _typeDefinitions(std::move(other._typeDefinitions))
        , _typeDefinitionIndexByName(
            std::move(other._typeDefinitionIndexByName))
    {

    }

    SyntaxTree::~SyntaxTree()
    {
    }

    void SyntaxTree::Add(TypeDefinition td)
    {
        auto i = _typeDefinitionIndexByName.find(td.Name());

        if (i == _typeDefinitionIndexByName.end())
        {
            _typeDefinitionIndexByName[td.Name()] = _typeDefinitions.size();
            _typeDefinitions.push_back(std::move(td));
        }
    }
}
