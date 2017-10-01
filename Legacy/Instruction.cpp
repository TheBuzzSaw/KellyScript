#include "Instruction.hpp"

namespace Kelly
{
    Instruction::Instruction()
    {
        _nextInstruction = 0;
    }

    Instruction::~Instruction()
    {
        delete _nextInstruction;
    }
}
