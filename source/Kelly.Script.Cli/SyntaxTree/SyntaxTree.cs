using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace Kelly.Script.Cli;

sealed class SyntaxTree : SyntaxNode
{
    public string? SourceFile { get; init; }
    public override void Write(StringBuilder builder, IndentLevel indentLevel)
    {
        throw new NotImplementedException();
    }

    public static SyntaxTree FromFile(
        string sourceFile,
        OperatorIndex operatorIndex,
        TokenInfo tokenInfo)
    {
        var sourceCodeUtf8 = File.ReadAllBytes(sourceFile);
        var tokens = new List<Token>();
        var openSandwiches = new List<Token>();
        var reader = SpanReader.Create(sourceCodeUtf8);
        int line = 1;
        int column = 1;

        while (true)
        {
            var token = reader.ReadToken(
                ref line,
                ref column,
                tokenInfo);
            tokens.Add(token);
            if (token.Type == TokenType.Eof)
                break;
            
            switch (token.Type)
            {
                case TokenType.OpenParen:
                case TokenType.OpenBrace:
                case TokenType.OpenBracket:
                    openSandwiches.Add(token);
                    break;
                case TokenType.CloseParen:
                    CloseSandwich(token, TokenType.OpenParen);
                    break;
                case TokenType.CloseBrace:
                    CloseSandwich(token, TokenType.OpenBrace);
                    break;
                case TokenType.CloseBracket:
                    CloseSandwich(token, TokenType.OpenBracket);
                    break;
            }
        }

        var result = new SyntaxTree
        {
            SourceFile = sourceFile
        };

        return result;

        void CloseSandwich(Token token, TokenType expectedTokenType)
        {
            if (openSandwiches.Exists(token => token.Type == expectedTokenType))
            {
                var openSandwich = openSandwiches.Pop();
                if (openSandwich.Type != expectedTokenType)
                {

                }
            }
            else
            {
                var left = expectedTokenType switch
                {
                    TokenType.OpenParen => '(',
                    TokenType.OpenBrace => '{',
                    TokenType.OpenBracket => '[',
                    _ => throw IllegalTokenType()
                };

                var right = token.Type switch
                {
                    TokenType.CloseParen => ')',
                    TokenType.CloseBrace => '}',
                    TokenType.CloseBracket => ']',
                    _ => throw IllegalTokenType()
                };

                throw new LexerException(
                    $"Line {token.Line} Column {token.Column}: Unexpected '{right}'. Missing '{left}'.");
            }

            static InvalidOperationException IllegalTokenType() => new("Illegal token type.");
        }
    }
}