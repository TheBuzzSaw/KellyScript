using System;
using System.Text;

namespace Kelly.Script.Cli;

sealed class OperatorInfo
{
    private readonly byte[] _syntaxUtf8;
    public string Syntax { get; }

    public ReadOnlySpan<byte> SyntaxUtf8 => _syntaxUtf8;
    public TokenType Token { get; }
    public int Precedence { get; }

    public OperatorInfo(
        string syntax,
        TokenType tokenType,
        int precedence)
    {
        Syntax = syntax;
        _syntaxUtf8 = Encoding.UTF8.GetBytes(syntax);
        Token = tokenType;
        Precedence = precedence;
    }

    public override string ToString() => $"{Syntax} ({Precedence})";
}