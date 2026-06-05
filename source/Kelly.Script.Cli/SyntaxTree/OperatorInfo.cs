using System.Collections.Frozen;
using System.Collections.Generic;
using System.Collections.Immutable;

namespace Kelly.Script.Cli;

sealed class OperatorInfo
{
    public FrozenDictionary<string, BinaryOperatorInfo> BinaryOperatorInfoBySyntax { get; }
    public FrozenDictionary<TokenType, BinaryOperatorInfo> BinaryOperatorInfoByTokenType { get; }
    public ImmutableArray<Associativity> AssociativityByPrecedence { get; }

    private OperatorInfo(
        IEnumerable<BinaryOperatorInfo> binaryOperators,
        IEnumerable<Associativity> associativityByPredence)
    {
        BinaryOperatorInfoBySyntax = binaryOperators.ToFrozenDictionary(bo => bo.Syntax);
        BinaryOperatorInfoByTokenType = BinaryOperatorInfoBySyntax.Values.ToFrozenDictionary(bo => bo.Token);
        AssociativityByPrecedence = associativityByPredence.ToImmutableArray();
    }

    public Associativity GetAssociativity(int precedence) => AssociativityByPrecedence[precedence];
    public bool IsRightToLeft(int precedence) =>
        AssociativityByPrecedence[precedence] == Associativity.RightToLeft;
    public bool IsLeftToRight(int precedence) =>
        AssociativityByPrecedence[precedence] == Associativity.LeftToRight;

    public static OperatorInfo Create()
    {
        var binaryOperators = new List<BinaryOperatorInfo>();
        var associativityByPrecedence = new List<Associativity>();
        var pairs = new List<KeyValuePair<string, int>>();
        var isRightToLeft = new List<bool>();

        // https://en.cppreference.com/cpp/language/operator_precedence

        {
            var p = NextPrecedence(Associativity.LeftToRight);
            binaryOperators.Add(new BinaryOperatorInfo(".", TokenType.DotOp, p));
        }

        {
            var p = NextPrecedence(Associativity.LeftToRight);
            binaryOperators.Add(new BinaryOperatorInfo("*", TokenType.Splat, p));
            binaryOperators.Add(new BinaryOperatorInfo("/", TokenType.Slash, p));
            binaryOperators.Add(new BinaryOperatorInfo("%", TokenType.Percent, p));
        }

        {
            var p = NextPrecedence(Associativity.LeftToRight);
            binaryOperators.Add(new BinaryOperatorInfo("+", TokenType.Plus, p));
            binaryOperators.Add(new BinaryOperatorInfo("-", TokenType.Minus, p));
        }

        {
            var p = NextPrecedence(Associativity.LeftToRight);
            binaryOperators.Add(new("<<", TokenType.ShiftLeft, p));
            binaryOperators.Add(new(">>", TokenType.ShiftRight, p));
        }

        {
            var p = NextPrecedence(Associativity.LeftToRight);
            binaryOperators.Add(new("<", TokenType.LessThan, p));
            binaryOperators.Add(new("<=", TokenType.LessThanOrEqual, p));
            binaryOperators.Add(new(">", TokenType.GreaterThan, p));
            binaryOperators.Add(new(">=", TokenType.GreaterThanOrEqual, p));
        }

        {
            var p = NextPrecedence(Associativity.LeftToRight);
            binaryOperators.Add(new("==", TokenType.Equal, p));
            binaryOperators.Add(new("!=", TokenType.NotEqual, p));
        }

        {
            var p = NextPrecedence(Associativity.LeftToRight);
            binaryOperators.Add(new("==", TokenType.Equal, p));
            binaryOperators.Add(new("!=", TokenType.NotEqual, p));
        }

        {
            var p = NextPrecedence(Associativity.LeftToRight);
            binaryOperators.Add(new("&", TokenType.BitwiseAnd, p));
        }

        {
            var p = NextPrecedence(Associativity.LeftToRight);
            binaryOperators.Add(new("^", TokenType.BitwiseXor, p));
        }

        {
            var p = NextPrecedence(Associativity.LeftToRight);
            binaryOperators.Add(new("|", TokenType.BitwiseOr, p));
        }

        {
            var p = NextPrecedence(Associativity.LeftToRight);
            binaryOperators.Add(new("and", TokenType.LogicalAnd, p));
            binaryOperators.Add(new("nand", TokenType.LogicalNand, p));
        }

        {
            var p = NextPrecedence(Associativity.LeftToRight);
            binaryOperators.Add(new("xor", TokenType.LogicalXor, p));
            binaryOperators.Add(new("xnor", TokenType.LogicalXnor, p));
        }

        {
            var p = NextPrecedence(Associativity.LeftToRight);
            binaryOperators.Add(new("or", TokenType.LogicalOr, p));
            binaryOperators.Add(new("nor", TokenType.LogicalNor, p));
        }

        {
            var p = NextPrecedence(Associativity.RightToLeft);
            binaryOperators.Add(new("=", TokenType.Assign, p));
            binaryOperators.Add(new("+=", TokenType.AssignSum, p));
            binaryOperators.Add(new("-=", TokenType.AssignDiff, p));
            binaryOperators.Add(new("*=", TokenType.AssignProduct, p));
            binaryOperators.Add(new("/=", TokenType.AssignQuotient, p));
            binaryOperators.Add(new("%=", TokenType.AssignMod, p));
            binaryOperators.Add(new("<<=", TokenType.AssignLeftShift, p));
            binaryOperators.Add(new(">>=", TokenType.AssignRightShift, p));
            binaryOperators.Add(new("&=", TokenType.AssignAnd, p));
            binaryOperators.Add(new("^=", TokenType.AssignXor, p));
            binaryOperators.Add(new("|=", TokenType.AssignOr, p));
        }
        
        var result = new OperatorInfo(binaryOperators, associativityByPrecedence);
        return result;

        int NextPrecedence(Associativity associativity)
        {
            var result = associativityByPrecedence.Count;
            associativityByPrecedence.Add(associativity);
            return result;
        }
    }

    private static class OldAssociativity
    {
        public const bool LeftToRight = false;
        public const bool RightToLeft = true;
    }
}