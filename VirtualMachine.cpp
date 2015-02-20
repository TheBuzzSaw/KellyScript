#include "VirtualMachine.hpp"

#include <windows.h>
#include <iostream>
#include <iomanip>
#include <vector>
#include <cstring>

namespace Kelly
{
    template<typename T>
    void Push(const void* source, uint8_t*& destination)
    {
        *reinterpret_cast<T*>(destination) = *reinterpret_cast<const T*>(source);
        destination += sizeof(T);
    }

    template<typename T>
    T Pop(uint8_t*& stackPointer)
    {
        stackPointer -= sizeof(T);
        return *reinterpret_cast<const T*>(stackPointer);
    }

    template<typename T>
    void PushLiteral(const uint8_t*& source, uint8_t*& destination)
    {
        Push<T>(source + 1, destination);
        source += sizeof(T);
    }

    template<typename T>
    void PushLocal(
        const uint8_t*& current,
        uint8_t*& stackPointer,
        uint8_t* framePointer)
    {
        uint16_t offset = *reinterpret_cast<const uint16_t*>(current + 1);
        current += 2;

        *reinterpret_cast<T*>(stackPointer) =
            *reinterpret_cast<const T*>(framePointer + offset);

        stackPointer += sizeof(T);
    }

    template<typename T>
    void PushCopy(uint8_t*& stackPointer)
    {
        *reinterpret_cast<T*>(stackPointer) =
            *reinterpret_cast<T*>(stackPointer - sizeof(T));

        stackPointer += sizeof(T);
    }

    template<typename T>
    void StoreLocal(
        const uint8_t*& current,
        uint8_t*& stackPointer,
        uint8_t* framePointer)
    {
        uint16_t offset = *reinterpret_cast<const uint16_t*>(current + 1);
        current += 2;

        *reinterpret_cast<T*>(framePointer + offset) = Pop<T>(stackPointer);
    }

    template<typename T>
    void Add(uint8_t*& stackPointer)
    {
        T a = Pop<T>(stackPointer);
        T b = Pop<T>(stackPointer);
        T c = a + b;
        Push<T>(&c, stackPointer);
    }

    void Run(const uint8_t* bytecodes)
    {
        std::vector<uint8_t> memStack(1024);
        uint8_t* stackPointer = memStack.data();
        uint8_t* framePointer = stackPointer;
        const uint8_t* current = bytecodes;

        std::cerr << std::hex;

        while (true)
        {
            using namespace Bytecodes;
            //std::cerr
            //  << "offset " << std::dec << (int)(current - bytecodes)
            //  << " bytecode " << std::hex << (int)(*current) << '\n';
            //::Sleep(500);

            switch(*current)
            {
                case Noop: break;

                case Jump:
                {
                    uint16_t offset =
                        *reinterpret_cast<const uint16_t*>(current + 1);

                    current = bytecodes + offset - 1;

                    break;
                }

                case Pop8:
                    stackPointer -= 1;
                    break;

                case Pop16:
                    stackPointer -= 2;
                    break;

                case Pop32:
                    stackPointer -= 4;
                    break;

                case Pop64:
                    stackPointer -= 8;
                    break;

                case PushLiteral8:
                    PushLiteral<uint8_t>(current, stackPointer);
                    break;

                case PushLiteral16:
                    PushLiteral<uint16_t>(current, stackPointer);
                    break;

                case PushLiteral32:
                    PushLiteral<uint32_t>(current, stackPointer);
                    break;

                case PushLiteral64:
                    PushLiteral<uint64_t>(current, stackPointer);
                    break;

                case PushLocal8:
                    PushLocal<uint8_t>(current, stackPointer, framePointer);
                    break;

                case PushLocal16:
                    PushLocal<uint16_t>(current, stackPointer, framePointer);
                    break;

                case PushLocal32:
                    PushLocal<uint32_t>(current, stackPointer, framePointer);
                    break;

                case PushLocal64:
                    PushLocal<uint64_t>(current, stackPointer, framePointer);
                    break;

                case PushCopy8:
                    PushCopy<uint8_t>(stackPointer);
                    break;

                case PushCopy16:
                    PushCopy<uint16_t>(stackPointer);
                    break;

                case PushCopy32:
                    PushCopy<uint32_t>(stackPointer);
                    break;

                case PushCopy64:
                    PushCopy<uint64_t>(stackPointer);
                    break;

                case StoreLocal8:
                    StoreLocal<uint8_t>(current, stackPointer, framePointer);
                    break;

                case StoreLocal16:
                    StoreLocal<uint16_t>(current, stackPointer, framePointer);
                    break;

                case StoreLocal32:
                    StoreLocal<uint32_t>(current, stackPointer, framePointer);
                    break;

                case StoreLocal64:
                    StoreLocal<uint64_t>(current, stackPointer, framePointer);
                    break;

                case AddS8:
                    Add<int8_t>(stackPointer);
                    break;

                case AddS16:
                    Add<int16_t>(stackPointer);
                    break;

                case AddS32:
                    Add<int32_t>(stackPointer);
                    break;

                case AddS64:
                    Add<int64_t>(stackPointer);
                    break;

                case JumpIfGE32:
                {
                    uint16_t offset =
                        *reinterpret_cast<const uint16_t*>(current + 1);

                    auto b = Pop<uint32_t>(stackPointer);
                    auto a = Pop<uint32_t>(stackPointer);

                    //std::cerr << "a " << a << " b " << b << '\n';

                    if (a >= b)
                        current = bytecodes + offset - 1;
                    else
                        current += 2;

                    break;
                }

                case OutS8:
                    std::cout << Pop<int8_t>(stackPointer) << std::endl;
                    break;

                case OutS16:
                    std::cout << Pop<int16_t>(stackPointer) << std::endl;
                    break;

                case OutS32:
                    std::cout << Pop<int32_t>(stackPointer) << std::endl;
                    break;

                case OutS64:
                    std::cout << Pop<int64_t>(stackPointer) << std::endl;
                    break;

                case Exit:
                    return;

                default:
                    std::cerr << "unrecognized byte code: " << (int)(*current) << '\n';
                    return;
            }

            ++current;
        }
    }
}
