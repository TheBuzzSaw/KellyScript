#ifndef STATE_HPP
#define STATE_HPP

#include "Instruction.hpp"
#include "Primitives.hpp"

namespace Kelly
{
    class State
    {
        public:
            State();
            virtual ~State();

            void Step();

        protected:
        private:
            Instruction* _currentInstruction;
            void* _stack;
            UInt8* _stackPointer;
    };
}

#endif
