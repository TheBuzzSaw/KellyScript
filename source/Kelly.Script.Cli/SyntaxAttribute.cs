using System;

namespace Kelly.Script.Cli;

[AttributeUsage(AttributeTargets.Field)]
sealed class SyntaxAttribute : Attribute
{
    public string Syntax { get; }

    public SyntaxAttribute(string syntax) => Syntax = syntax;
}