using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Text;

namespace Kelly.Script.Cli;

static class Program
{
    private static void Main(string[] args)
    {
        try
        {
            var oi = OperatorIndex.Create();
            if (args.Length == 0)
            {
                foreach (var info in oi.BinaryOperators)
                {
                    var a = oi.IsLeftToRight(info.Precedence) ? "left-to-right" : "right-to-left";
                    Console.WriteLine($"{info} : {a}");
                }
                return;
            }

            var ti = TokenInfo.Create();

            var path = args[0];
            var x = args.Length;
            Console.WriteLine($"Tokenizing {path}");
            var code = File.ReadAllBytes(path);
            var reader = SpanReader.Create(code);
            int line = 1;
            int column = 1;
            var openSandwiches = new List<Token>();
            while (true)
            {
                var token = reader.ReadToken(ref line, ref column, ti);
                if (token.Type == TokenType.Eof)
                    break;
                var text = Encoding.UTF8.GetString(code.AsSpan(token.Start, token.Length));
                Console.WriteLine($"Line {token.Line} Col {token.Column} ({token.Type}): {text}");

                var openToken = default(Token);
                switch (token.Type)
                {
                    case TokenType.OpenBrace:
                    case TokenType.OpenBracket:
                    case TokenType.OpenParen:
                        openSandwiches.Add(token);
                        break;
                    case TokenType.CloseBrace:
                        if (!openSandwiches.Exists(t => t.Type == TokenType.OpenBrace))
                            throw new InvalidOperationException("Lexer Error: Found '}'. Missing '{'.");
                        openToken = openSandwiches[^1];
                        if (openToken.Type != TokenType.OpenBrace)
                        {
                            if (openToken.Type == TokenType.OpenBracket)
                                throw new InvalidOperationException("Lexer Error: Missing ']'.");
                            if (openToken.Type == TokenType.OpenParen)
                                throw new InvalidOperationException("Lexer Error: Missing ')'.");
                            throw new InvalidOperationException("Lexer Error: Missing unknown closer.");
                        }
                        openSandwiches.RemoveAt(^1);
                        break;
                    case TokenType.CloseBracket:
                        if (!openSandwiches.Exists(t => t.Type == TokenType.OpenBracket))
                            throw new InvalidOperationException("Lexer Error: Found ']'. Missing '['.");
                        openToken = openSandwiches[^1];
                        if (openToken.Type != TokenType.OpenBracket)
                        {
                            if (openToken.Type == TokenType.OpenBrace)
                                throw new InvalidOperationException("Lexer Error: Missing '}'.");
                            if (openToken.Type == TokenType.OpenParen)
                                throw new InvalidOperationException("Lexer Error: Missing ')'.");
                            throw new InvalidOperationException("Lexer Error: Missing unknown closer.");
                        }
                        openSandwiches.RemoveAt(^1);
                        break;
                    case TokenType.CloseParen:
                        if (!openSandwiches.Exists(t => t.Type == TokenType.OpenParen))
                            throw new InvalidOperationException("Lexer Error: Found ')'. Missing '('.");
                        openToken = openSandwiches[^1];
                        if (openToken.Type != TokenType.OpenParen)
                        {
                            if (openToken.Type == TokenType.OpenBrace)
                                throw new InvalidOperationException("Lexer Error: Missing '}'.");
                            if (openToken.Type == TokenType.OpenBracket)
                                throw new InvalidOperationException("Lexer Error: Missing ']'.");
                            throw new InvalidOperationException("Lexer Error: Missing unknown closer.");
                        }
                        openSandwiches.RemoveAt(^1);
                        break;
                }
            }

            if (0 < openSandwiches.Count)
            {
                var openToken = openSandwiches[^1];
                var c = openToken.Type switch
                {
                    TokenType.OpenBrace => '}',
                    TokenType.OpenBracket => ']',
                    TokenType.OpenParen => ')',
                    _ => '?'
                };
                throw new InvalidOperationException($"Lexer Error: Missing '{c}'.");
            }
        }
        catch (Exception ex)
        {
            Console.ResetColor();
            Console.ForegroundColor = ConsoleColor.Red;
            Console.WriteLine();
            Console.WriteLine(ex);
            Console.WriteLine();
            Console.ResetColor();
        }
    }
}
