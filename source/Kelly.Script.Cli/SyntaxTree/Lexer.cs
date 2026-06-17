using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace Kelly.Script.Cli;

sealed class Lexer
{
    private static ReadOnlySpan<char> Whitespace => " \r\t";
    
    private readonly List<char> _buffer = [];
    private readonly TokenInfo _tokenInfo;

    public ReadOnlySpan<char> Buffer => CollectionsMarshal.AsSpan(_buffer);

    public Lexer(TokenInfo tokenInfo)
    {
        _tokenInfo = tokenInfo;
    }

    public Token GetNextToken(
        ref SpanReader<char> reader,
        ref int line,
        ref int column)
    {
        _buffer.Clear();

        while (reader.TryPeek(out var first))
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
            if (reader.TryMatch('\''))
            {
                _buffer.Add('\0');
            }
            else
            {
                var body = reader.ReadCodePointLiteral();
                _buffer.Add(body);

                if (!reader.TryMatch('\''))
                    throw new LexerException($"Line {line} Column {column}: Expected ' after code point.");
            }
        }

        if (token.Length < 1)
            token.Length = 1;
        return token;
    }
}