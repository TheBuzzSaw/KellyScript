#ifndef SYNTAXTREE_HPP
#define SYNTAXTREE_HPP

#include "View.hpp"
#include "TypeDefinition.hpp"
#include "Token.hpp"
#include <vector>
#include <unordered_map>

namespace Kelly
{
    class SyntaxTree
    {
        public:
            SyntaxTree(View<char*> arguments);
            SyntaxTree(SyntaxTree&& other);
            ~SyntaxTree();

            SyntaxTree& operator=(SyntaxTree&& other);

        protected:
        private:
            SyntaxTree(const SyntaxTree&) = delete;
            SyntaxTree& operator=(const SyntaxTree&) = delete;

            void Add(TypeDefinition td);

            std::vector<Token> _tokens;
            std::vector<TypeDefinition> _typeDefinitions;
            std::unordered_map<std::string,
                decltype(_typeDefinitions)::size_type>
                    _typeDefinitionIndexByName;
    };
}

#endif
