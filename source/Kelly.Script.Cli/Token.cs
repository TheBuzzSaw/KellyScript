using System;
using System.Buffers;

namespace Kelly.Script.Cli;

public struct Token
{
    public TokenType Type;
    public int Start;
    public int Length;
    public int Line;
    public int Column;

    private const int LowerFlag = 1 << 5;

    public static bool IsDigit(int c) => '0' <= c && c <= '9';
    public static bool IsLower(int c) => 'a' <= c && c <= 'z';
    public static bool IsUpper(int c) => 'A' <= c && c <= 'Z';
    public static bool IsAlpha(int c) => IsUpper(c & ~LowerFlag);
    public static bool IsAlphanumeric(int c) => IsDigit(c) || IsAlpha(c);
    public static bool IsIdentifierFriendly(int c) => c == '_' || IsAlphanumeric(c);
}

public static class TokenExtensions
{
    private static readonly SearchValues<byte> Whitespace = SearchValues.Create(" \t\r\n"u8);

    public static ref SpanReader<byte> ParseToken(
        ref this SpanReader<byte> reader,
        ref int line,
        ref int column,
        out Token token)
    {
        if (!reader.HasMore)
        {
            token = new Token
            {
                Type = TokenType.Eof,
                Start = reader.Span.Length,
                Length = 0,
                Line = line,
                Column = column
            };
        }
        else
        {
            token = new Token
            {
                Start = reader.Position,
                Length = 1,
                Line = line,
                Column = column++
            };
            var first = reader.Chomp();
            if (Whitespace.Contains(first))
            {
                token.Type = TokenType.Gap;
                if (first == '\n')
                {
                    ++line;
                    column = 1;
                }

                while (reader.HasMore && Whitespace.Contains(reader.Peek()))
                {
                    ++token.Length;

                    if (reader.Chomp() == '\n')
                    {
                        ++line;
                        column = 1;
                    }
                    else
                    {
                        ++column;
                    }
                }
            }
            else if (first == '_' || Token.IsAlpha(first))
            {
                token.Type = TokenType.Identifier;
                while (reader.HasMore && Token.IsIdentifierFriendly(reader.Peek()))
                {
                    ++reader.Position;
                    ++token.Length;
                    ++column;
                }
            }
            else if (Token.IsDigit(first))
            {
                token.Type = TokenType.Integer;
                while (reader.HasMore && Token.IsDigit(reader.Peek()))
                {
                    ++reader.Position;
                    ++token.Length;
                    ++column;
                }
            }
            else
            {
                token.Type = (int)first switch
                {
                    '`' => TokenType.Grave,
                    '~' => TokenType.Tilde,
                    '!' => TokenType.Bang,
                    '@' => TokenType.At,
                    '#' => TokenType.Pound,
                    '$' => TokenType.Dollar,
                    '%' => TokenType.Percent,
                    '^' => TokenType.Caret,
                    '&' => TokenType.Ampersand,
                    '*' => TokenType.Asterisk,
                    '(' => TokenType.OpenParen,
                    ')' => TokenType.CloseParen,
                    '-' => TokenType.Minus,
                    '+' => TokenType.Plus,
                    '=' => TokenType.Equals,
                    '[' => TokenType.OpenBracket,
                    ']' => TokenType.CloseBracket,
                    '{' => TokenType.OpenBrace,
                    '}' => TokenType.CloseBrace,
                    '.' => TokenType.Dot,
                    ',' => TokenType.Comma,
                    '/' => TokenType.Slash,
                    ':' => TokenType.Colon,
                    ';' => TokenType.Semicolon,
                    _ => TokenType.IllegalCodePoint
                };
            }
        }

        return ref reader;
    }
}
