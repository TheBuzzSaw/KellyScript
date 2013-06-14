#ifndef STATE_HPP
#define STATE_HPP

#include "Instruction.hpp"
#include "Primitives.hpp"
#include "Identifier.hpp"
#include <map>

namespace Kelly
{
    class State
    {
        public:
            State();
            ~State();

            void LoadFromString(const char* script);
            bool LoadFromFile(const char* path);

            void Step();

        protected:
        private:
            State(const State& other);
            State(State&& other);

            void PrepareIdentifiers();

            Instruction* _currentInstruction;
            void* _stack;
            UInt8* _stackPointer;
    };
}

#endif
