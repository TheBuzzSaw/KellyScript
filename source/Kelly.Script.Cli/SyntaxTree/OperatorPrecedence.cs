using System.Collections.Frozen;
using System.Collections.Generic;

namespace Kelly.Script.Cli;

readonly struct OperatorPrecedence
{
    public int Precedence { get; }
    public bool RightToLeft { get; }
    public bool LeftToRight => !RightToLeft;

    public OperatorPrecedence(int precedence, bool rightToLeft)
    {
        Precedence = precedence;
        RightToLeft = rightToLeft;
    }

    private string GetAssociativityString() => RightToLeft ? "right-to-left" : "left-to-right";
    public override string ToString() => $"precedence {Precedence} associativity {GetAssociativityString()}";

    public static FrozenDictionary<string, OperatorPrecedence> CreateMapping()
    {
        var pairs = new List<KeyValuePair<string, OperatorPrecedence>>();
        var precedence = 0;

        // https://en.cppreference.com/cpp/language/operator_precedence
        
        {
            var op = new OperatorPrecedence(
                ++precedence, Associativity.LeftToRight);
            Add(".", op);
        }

        {
            var op = new OperatorPrecedence(
                ++precedence, Associativity.LeftToRight);
            Add("*", op);
            Add("/", op);
            Add("%", op);
        }

        {
            var op = new OperatorPrecedence(
                ++precedence, Associativity.LeftToRight);
            Add("+", op);
            Add("-", op);
        }

        {
            var op = new OperatorPrecedence(
                ++precedence, Associativity.LeftToRight);
            Add("<<", op);
            Add(">>", op);
        }

        {
            var op = new OperatorPrecedence(
                ++precedence, Associativity.LeftToRight);
            Add("<", op);
            Add("<=", op);
            Add(">", op);
            Add(">=", op);
        }

        {
            var op = new OperatorPrecedence(
                ++precedence, Associativity.LeftToRight);
            Add("==", op);
            Add("!=", op);
        }

        {
            var op = new OperatorPrecedence(
                ++precedence, Associativity.LeftToRight);
            Add("&", op);
        }

        {
            var op = new OperatorPrecedence(
                ++precedence, Associativity.LeftToRight);
            Add("^", op);
        }

        {
            var op = new OperatorPrecedence(
                ++precedence, Associativity.LeftToRight);
            Add("|", op);
        }

        {
            var op = new OperatorPrecedence(
                ++precedence, Associativity.LeftToRight);
            Add("and", op);
            Add("nand", op);
        }

        {
            var op = new OperatorPrecedence(
                ++precedence, Associativity.LeftToRight);
            Add("xor", op);
            Add("xnor", op);
        }

        {
            var op = new OperatorPrecedence(
                ++precedence, Associativity.LeftToRight);
            Add("or", op);
            Add("nor", op);
        }

        {
            var op = new OperatorPrecedence(
                ++precedence, Associativity.RightToLeft);
            Add("=", op);
            Add("+=", op);
            Add("-=", op);
            Add("*=", op);
            Add("/=", op);
            Add("%=", op);
            Add("<<=", op);
            Add(">>=", op);
            Add("&=", op);
            Add("^=", op);
            Add("|=", op);
        }

        var result = pairs.ToFrozenDictionary();
        return result;

        void Add(string syntax, OperatorPrecedence operatorPrecedence)
        {
            pairs.Add(
                KeyValuePair.Create(
                    syntax, operatorPrecedence));
        }
    }

    private static class Associativity
    {
        public const bool LeftToRight = false;
        public const bool RightToLeft = true;
    }
}