#ifndef IDENTIFIER_HPP
#define IDENTIFIER_HPP

#include <string>

namespace Kelly
{
    class Identifier
    {
        public:
            typedef int TypeHandle;

            static const TypeHandle Keyword = 1 << 0;
            static const TypeHandle DataType = 1 << 1;
            static const TypeHandle Variable = 1 << 2;
            static const TypeHandle Package = 1 << 3;

            Identifier(std::string name, TypeHandle type, Identifier* package);
            Identifier(const Identifier& other);
            virtual ~Identifier();

            inline std::string Name() const { return _name; }
            inline bool IsType(TypeHandle type) const { return _type & type; }

        protected:
        private:
            std::string _name;
            TypeHandle _type;
            Identifier* _package;
    };
}

#endif
