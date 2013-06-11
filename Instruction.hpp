#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

namespace Kelly
{
    class Instruction
    {
        public:
            Instruction();
            virtual ~Instruction();

            virtual void Execute() = 0;

            inline Instruction* NextInstruction()
            {
                return _nextInstruction;
            }

        protected:
            inline void NextInstruction(Instruction* instruction)
            {
                _nextInstruction = instruction;
            }

        private:
            Instruction* _nextInstruction;
    };
}

#endif
