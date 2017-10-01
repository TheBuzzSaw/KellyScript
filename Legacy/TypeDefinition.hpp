#ifndef TYPEDEFINITION_HPP
#define TYPEDEFINITION_HPP

#include <string>

namespace Kelly
{
    class TypeDefinition
    {
        public:
            TypeDefinition(std::string name, int byteCount);
            TypeDefinition(TypeDefinition&& other);
            ~TypeDefinition();

            TypeDefinition& operator=(TypeDefinition&& other);

            inline int ByteCount() const { return _byteCount; }
            inline const std::string& Name() const { return _name; }

        protected:
        private:
            TypeDefinition(const TypeDefinition&) = delete;
            TypeDefinition& operator=(const TypeDefinition&) = delete;

            std::string _name;
            int _byteCount;
    };
}

#endif
