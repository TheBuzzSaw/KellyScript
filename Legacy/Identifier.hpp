#ifndef IDENTIFIER_HPP
#define IDENTIFIER_HPP

#include <string>
#include <map>

namespace Kelly
{
    class Identifier
    {
        public:
            enum class Type : int
            {
                Keyword = 1 << 0,
                DataType = 1 << 1,
                Function = 1 << 2,
                Package = 1 << 3,
                Variable = 1 << 4
            };

            Identifier(std::string name, Type type,
                Identifier* parent = 0);
            virtual ~Identifier();

            inline std::string Name() const { return _name; }
            inline bool Is(Type type) const { return _type & (int)type; }

            Identifier* Child(const std::string& name) const;

        protected:
        private:
            Identifier(const Identifier& other);
            Identifier(Identifier&& other);
            Identifier& operator=(const Identifier& other);
            Identifier& operator=(Identifier&& other);

            std::string _name;
            int _type;
            Identifier* _parent;
            std::map<std::string, Identifier*> _childrenByName;
    };
}

#endif
