#ifndef PROGRAM_HPP
#define PROGRAM_HPP

#include "Instruction.hpp"

namespace Kelly
{
    class Program
    {
        public:
            Program();
            ~Program();

            void Load(const char* input);
            void Run();

        protected:
        private:
            Instruction* _firstInstruction;
    };
}

#endif
