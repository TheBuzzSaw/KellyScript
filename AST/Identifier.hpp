#ifndef IDENTIFIER_HPP
#define IDENTIFIER_HPP

#include <string>

namespace Kelly
{
    class Identifier
    {
        public:
            Identifier(std::string name);
            virtual ~Identifier();

            inline std::string Name() const { return _name; }

        protected:
        private:
            std::string _name;
    };
}

#endif
