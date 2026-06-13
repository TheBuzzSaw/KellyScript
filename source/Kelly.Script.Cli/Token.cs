using System;
using System.Buffers;
using System.Text;

namespace Kelly.Script.Cli;

struct Token
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

static class TokenExtensions
{
    private static readonly SearchValues<byte> Whitespace = SearchValues.Create(" \t\r\n"u8);

    public static Token ReadToken(
        ref this SpanReader<byte> reader,
        ref int line,
        ref int column,
        OperatorIndex operatorIndex,
        TokenInfo tokenInfo)
    {
        var first = reader.PeekOrDefault();
        while (Whitespace.Contains(first))
        {
            if (first == '\n')
            {
                ++line;
                column = 1;
            }
            else
            {
                ++column;
            }

            ++reader.Position;
            first = reader.PeekOrDefault();
        }

        var result = new Token
        {
            Start = reader.Position,
            Line = line,
            Column = column
        };

        if (reader.WasConsumed)
        {
            result.Type = TokenType.Eof;
            return result;
        }

        if (first == '_' || Token.IsAlpha(first))
        {
            ++reader.Position;
            ++result.Length;
            ++column;
            result.Type = TokenType.Identifier;
            while (Token.IsIdentifierFriendly(reader.PeekOrDefault()))
            {
                ++reader.Position;
                ++result.Length;
                ++column;
            }

            var identifier = Encoding.UTF8.GetString(reader.Span.Slice(result.Start, result.Length));
            if (tokenInfo.TokenTypesByKeyword.TryGetValue(identifier, out var tokenType))
                result.Type = tokenType;
        }
        else if (Token.IsDigit(first))
        {
            ++reader.Position;
            ++result.Length;
            ++column;
            result.Type = TokenType.Integer;
            while (Token.IsDigit(reader.PeekOrDefault()))
            {
                ++reader.Position;
                ++result.Length;
                ++column;
            }
        }
        else if (reader.Pending.StartsWith("//"u8))
        {
            result.Type = TokenType.CommentLine;
            var end = reader.Pending.IndexOf((byte)'\n');
            if (end == -1)
            {
                result.Length = reader.Pending.Length;
                reader.Position = reader.Span.Length;
            }
            else
            {
                result.Length = end;
                column += result.Length;
                reader.Position += end;
            }
        }
        else
        {
            var tt = (int)first switch
            {
                '(' => TokenType.OpenParen,
                ')' => TokenType.CloseParen,
                '[' => TokenType.OpenBracket,
                ']' => TokenType.CloseBracket,
                '{' => TokenType.OpenBrace,
                '}' => TokenType.CloseBrace,
                _ => TokenType.None
            };

            if (tt != TokenType.None)
            {
                ++reader.Position;
                ++column;
                result.Type = tt;
                result.Length = 1;
            }
            else
            {
                var pending = reader.Pending;
                var matchLength = 0;
                result.Type = TokenType.IllegalCodePoint;
                foreach (var bo in operatorIndex.BinaryOperators)
                {
                    var syntax = bo.SyntaxUtf8;
                    if (matchLength < syntax.Length && pending.StartsWith(syntax))
                    {
                        matchLength = syntax.Length;
                        result.Type = bo.Token;
                    }
                }

                if (matchLength == 0)
                {
                    var c = (char)reader.PeekOrDefault();
                    matchLength = 1;
                }

                reader.Position += matchLength;
                result.Length = matchLength;
                column += matchLength;
            }
        }

        return result;
    }
}
