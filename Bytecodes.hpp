#ifndef BYTECODES_HPP
#define BYTECODES_HPP

#include <cstdint>

namespace Kelly
{
    namespace Bytecodes
    {
        const uint8_t Noop = 0x00;
        const uint8_t Exit = 0x01;
        const uint8_t Jump = 0x10;

        const uint8_t PushLiteral8 = 0x20;
        const uint8_t PushLiteral16 = 0x21;
        const uint8_t PushLiteral32 = 0x22;
        const uint8_t PushLiteral64 = 0x23;

        const uint8_t PushLocal8 = 0x24;
        const uint8_t PushLocal16 = 0x25;
        const uint8_t PushLocal32 = 0x26;
        const uint8_t PushLocal64 = 0x27;

        const uint8_t AddS8 = 0x30;
        const uint8_t AddS16 = 0x31;
        const uint8_t AddS32 = 0x32;
        const uint8_t AddS64 = 0x33;

        const uint8_t AddU8 = 0x34;
        const uint8_t AddU16 = 0x35;
        const uint8_t AddU32 = 0x36;
        const uint8_t AddU64 = 0x37;

        const uint8_t SubS8 = 0x38;
        const uint8_t SubS16 = 0x39;
        const uint8_t SubS32 = 0x3a;
        const uint8_t SubS64 = 0x3b;

        const uint8_t SubU8 = 0x3c;
        const uint8_t SubU16 = 0x3d;
        const uint8_t SubU32 = 0x3e;
        const uint8_t SubU64 = 0x3f;

        const uint8_t OutS8 = 0xe0;
        const uint8_t OutS16 = 0xe1;
        const uint8_t OutS32 = 0xe2;
        const uint8_t OutS64 = 0xe3;
    }
}

#endif

