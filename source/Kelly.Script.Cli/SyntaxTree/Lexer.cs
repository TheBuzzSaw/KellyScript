using System;
using System.Collections.Generic;
using System.Numerics;
using System.Runtime.InteropServices;
using System.Text;

namespace Kelly.Script.Cli;

sealed class Lexer
{
    private static ReadOnlySpan<char> Whitespace => " \r\t";
    
    private readonly List<char> _buffer = [];
    private readonly List<Token> _openSandwiches = [];
    private readonly TokenInfo _tokenInfo;

    public ReadOnlySpan<char> Buffer => CollectionsMarshal.AsSpan(_buffer);
    public Rune CodePoint { get; private set; }
    public BigInteger Integer { get; private set; }

    public Lexer(TokenInfo tokenInfo)
    {
        _tokenInfo = tokenInfo;
    }

    public Token GetNextToken(
        ref SpanReader<char> reader,
        ref int line,
        ref int column)
    {
        var startingPosition = reader.Position;
        _buffer.Clear();
        CodePoint = default;
        Integer = BigInteger.Zero;
        char first;

        while (reader.TryPeek(out first))
        {
            if (Whitespace.Contains(first))
            {
                ++column;
                ++reader.Position;
                continue;
            }

            if (first == '\n')
            {
                ++line;
                column = 1;
                ++reader.Position;
                continue;
            }

            break;
        }

        var token = new Token
        {
            Type = TokenType.IllegalCodePoint,
            Start = reader.Position,
            Line = line,
            Column = column
        };

        if (reader.WasConsumed)
        {
            token.Type = TokenType.Eof;
            return token;
        }

        if (reader.StartsWith("//"))
        {
            var end = reader.Pending.IndexOf('\n');
            if (end == -1)
            {
                end = reader.Pending.Length;
                column += end;
            }
            else
            {
                ++end;
                ++line;
                column = 1;
            }
            reader.Position += end;
            token.Type = TokenType.CommentLine;
            token.Length = reader.Position - token.Start;
            return token;
        }

        if (reader.StartsWith("/*"))
        {
            var remaining = reader.Pending[2..];
            var openCount = 1;
            while (0 < openCount)
            {
                var index = remaining.IndexOf('/');
                if (index == -1)
                {
                    remaining = default;
                    break;
                }

                if (0 < index && remaining[index - 1] == '*')
                {
                    --openCount;
                    remaining = remaining.Slice(index + 1);
                }
                else if (index + 1 < remaining.Length && remaining[index + 1] == '*')
                {
                    ++openCount;
                    remaining = remaining.Slice(index + 2);
                }
                else
                {
                    remaining = remaining.Slice(index + 1);
                }
            }

            var end = reader.Pending.Length - remaining.Length;
            var commentBlock = reader.Pending[..end];
            reader.Position += end;
            var newLineCount = commentBlock.Count('\n');
            if (0 < newLineCount)
            {
                line += newLineCount;
                column = 1;

                var index = commentBlock.LastIndexOf('\n') + 1;
                column += commentBlock.Length - index;
            }
            else
            {
                column += commentBlock.Length;
            }
            token.Type = TokenType.CommentBlock;
            token.Length = reader.Position - token.Start;
            return token;
        }

        if (reader.TryMatch('\''))
        {
            column += 2;
            if (reader.TryMatch('\''))
            {
                _buffer.Add('\0');
            }
            else
            {
                CodePoint = reader.ReadCodePointLiteral();

                if (!reader.TryMatch('\''))
                    throw new LexerException($"Line {line} Column {column}: Expected ' after code point.");
                ++column;
            }
            token.Type = TokenType.LiteralCodePoint;
            token.Length = reader.Position - token.Start;
            return token;
        }

        if (reader.TryMatch('"'))
        {
            ++column;
            while (!reader.TryMatch('"'))
            {
                if (reader.WasConsumed)
                    throw new LexerException($"Line {line} Column {column}: Expected \" to end string literal.");
                var codePoint = reader.ReadCodePointLiteral();
                _buffer.Add(codePoint);
                ++column;
            }
            ++column;
            token.Type = TokenType.LiteralString;
            token.Length = reader.Position - token.Start;
            return token;
        }

        if (Token.IsDigit(first))
        {
            // TODO: Parse floats.
            token.Type = TokenType.LiteralInteger;
            while (true)
            {
                var next = reader.PeekOrDefault();
                if (Token.IsDigit(next))
                {
                    _buffer.Add(reader.Chomp());
                    ++column;
                    ++token.Length;
                }
                else
                {
                    break;
                }
            }
            Integer = BigInteger.Parse(Buffer);
            return token;
        }

        if (Token.IsAlpha(first) || first == '_')
        {
            token.Type = TokenType.Identifier;
            ++token.Length;
            ++reader.Position;
            ++column;
            _buffer.Add(first);

            while (Token.IsIdentifierFriendly(reader.PeekOrDefault()))
            {
                _buffer.Add(reader.Chomp());
                ++token.Length;
                ++column;
            }

            var lookup = _tokenInfo.TokenTypeBySyntax.GetAlternateLookup<ReadOnlySpan<char>>();
            if (lookup.TryGetValue(Buffer, out var keywordToken))
                token.Type = keywordToken;
            return token;
        }

        // Safeguard to avoid infinite loop.
        if (!reader.WasConsumed && reader.Position <= startingPosition)
            reader.Position = startingPosition + 1;
        return token;
    }
}