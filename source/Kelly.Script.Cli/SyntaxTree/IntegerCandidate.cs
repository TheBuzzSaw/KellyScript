using System;
using System.Numerics;

namespace Kelly.Script.Cli;

[Flags]
enum IntegerCandidate
{
    None,
    I8 = 1 << 0,
    I16 = 1 << 1,
    I32 = 1 << 2,
    I64 = 1 << 3,
    I128 = 1 << 4,
    U8 = 1 << 5,
    U16 = 1 << 6,
    U32 = 1 << 7,
    U64 = 1 << 8,
    U128 = 1 << 9
}

static class IntegerCandidateExtensions
{
    private const IntegerCandidate AllSigned =
        IntegerCandidate.I8 |
        IntegerCandidate.I16 |
        IntegerCandidate.I32 |
        IntegerCandidate.I64 |
        IntegerCandidate.I128;
    private const IntegerCandidate AllUnsigned =
        IntegerCandidate.U8 |
        IntegerCandidate.U16 |
        IntegerCandidate.U32 |
        IntegerCandidate.U64 |
        IntegerCandidate.U128;

    public static IntegerCandidate GetCandidates(this BigInteger n)
    {
        var result = IntegerCandidate.None;
        if (0 <= n)
        {
            result = AllSigned | AllUnsigned;
            CheckMax(sbyte.MaxValue, IntegerCandidate.I8);
            CheckMax(byte.MaxValue, IntegerCandidate.U8);
            CheckMax(short.MaxValue, IntegerCandidate.I16);
            CheckMax(ushort.MaxValue, IntegerCandidate.U16);
            CheckMax(int.MaxValue, IntegerCandidate.I32);
            CheckMax(uint.MaxValue, IntegerCandidate.U32);
            CheckMax(long.MaxValue, IntegerCandidate.I64);

            if (ulong.MaxValue < n)
                result &= ~IntegerCandidate.U64;
            
            if (Int128.MaxValue < n)
                result &= ~IntegerCandidate.I128;
            
            if (UInt128.MaxValue < n)
                result &= ~IntegerCandidate.U128;
        }
        else
        {
            result = AllSigned;
            CheckMin(sbyte.MinValue, IntegerCandidate.I8);
            CheckMin(short.MinValue, IntegerCandidate.I16);
            CheckMin(int.MinValue, IntegerCandidate.I32);
            CheckMin(long.MinValue, IntegerCandidate.I64);

            if (n < Int128.MinValue)
                result &= ~IntegerCandidate.I128;
        }
        return result;

        void CheckMax(long limit, IntegerCandidate flag)
        {
            if (limit < n)
                result &= ~flag;
        }

        void CheckMin(long limit, IntegerCandidate flag)
        {
            if (n < limit)
                result &= ~flag;
        }
    }
}