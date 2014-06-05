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
            static State FromString(const char* source);
            static State FromFile(const char* path);

            State();
            State(State&& other);
            ~State();

            State& operator=(State&& other);

            void Step();

            bool IsReady() const { return true; }

        protected:
        private:
            State(const State&) = delete;
            State& operator=(const State&) = delete;

            void PrepareIdentifiers();

            Instruction* _currentInstruction;
            void* _stack;
            UInt8* _stackPointer;
    };
}

#endif
