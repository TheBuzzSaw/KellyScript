#ifndef IDENTIFIER_HPP
#define IDENTIFIER_HPP

#include <string>
#include <map>

namespace Kelly
{
    typedef int TypeHandle;

    static const TypeHandle KeywordType = 1 << 0;
    static const TypeHandle DataTypeType = 1 << 1;
    static const TypeHandle VariableType = 1 << 2;
    static const TypeHandle PackageType = 1 << 3;

    class Identifier
    {
        public:
            Identifier(std::string name, TypeHandle type,
                Identifier* parent = 0);
            virtual ~Identifier();

            inline std::string Name() const { return _name; }
            inline bool IsType(TypeHandle type) const { return _type & type; }

            Identifier* Child(const std::string& name) const;

        protected:
        private:
            Identifier(const Identifier& other);
            Identifier(Identifier&& other);
            Identifier& operator=(const Identifier& other);
            Identifier& operator=(Identifier&& other);

            std::string _name;
            TypeHandle _type;
            Identifier* _parent;
            std::map<std::string, Identifier*> _childrenByName;
    };
}

#endif
