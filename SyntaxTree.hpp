#ifndef SYNTAXTREE_HPP
#define SYNTAXTREE_HPP

#include "TypeDefinition.hpp"
#include <vector>
#include <unordered_map>

namespace Kelly
{
    class SyntaxTree
    {
        public:
            SyntaxTree();
            SyntaxTree(SyntaxTree&& other);
            ~SyntaxTree();

            SyntaxTree& operator=(SyntaxTree&& other);

        protected:
        private:
            SyntaxTree(const SyntaxTree&) = delete;
            SyntaxTree& operator=(const SyntaxTree&) = delete;

            void Add(TypeDefinition td);

            std::vector<TypeDefinition> _typeDefinitions;
            std::unordered_map<std::string,
                decltype(_typeDefinitions)::size_type>
                    _typeDefinitionIndexByName;
    };
}

#endif
