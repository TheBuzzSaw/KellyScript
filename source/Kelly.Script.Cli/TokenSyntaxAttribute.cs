using System;

namespace Kelly.Script.Cli;

[AttributeUsage(AttributeTargets.Field)]
sealed class TokenSyntaxAttribute : Attribute
{
    public string Syntax { get; }

    public TokenSyntaxAttribute(string syntax) => Syntax = syntax;
}