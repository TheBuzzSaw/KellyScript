using System;
using System.Buffers;
using System.Collections.Generic;
using System.Text;

namespace Kelly.Script.Cli;

struct Token
{
    public TokenType Type;
    public int Start;
    public int Length;
    public int Line;
    public int Column;

    public readonly ReadOnlySpan<byte> GetSlice(ReadOnlySpan<byte> span) => span.Slice(Start, Length);

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
    private static readonly SearchValues<byte> OpSymbols = SearchValues.Create("`~!@#$%^&*()-=_+[]{}\\|;:'\",.<>/?"u8);

    public static List<Token> Lex(ReadOnlySpan<byte> sourceCodeUtf8, TokenInfo tokenInfo)
    {
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
        
        return tokens;

        void CloseSandwich(Token token, TokenType expectedOpenSandwichType)
        {
            if (openSandwiches.Exists(token => token.Type == expectedOpenSandwichType))
            {
                var openSandwich = openSandwiches.Pop();
                if (openSandwich.Type != expectedOpenSandwichType)
                {
                    var missing = expectedOpenSandwichType switch
                    {
                        TokenType.OpenParen => ')',
                        TokenType.OpenBrace => '}',
                        TokenType.OpenBracket => ']',
                        _ => throw new InvalidOperationException("Illegal token type.")
                    };
                    throw new LexerException(
                        $"Line {token.Line} Column {token.Column}: Missing '{missing}' originally opened at {openSandwich.Line}:{openSandwich.Column}.");
                }
            }
            else
            {
                var left = tokenInfo.SyntaxByTokenType[expectedOpenSandwichType];
                var right = tokenInfo.SyntaxByTokenType[token.Type];

                throw new LexerException(
                    $"Line {token.Line} Column {token.Column}: Found unexpected '{right}'. Missing '{left}'.");
            }
        }
    }

    public static Rune ReadCodePointLiteral(this ref SpanReader<char> reader)
    {
        var rune = reader.DecodeRuneFromUtf16();
        if (rune.Value == '\\')
        {
            // TODO: Add sequences for unicode values.
            var escapeCharacter = reader.DecodeRuneFromUtf16();
            Rune result = escapeCharacter.Value switch
            {
                '\'' => new('\''),
                '"' => new('"'),
                'n' => new('\n'),
                '\\' => new('\\'),
                'r' => new('\r'),
                't' => new('\t'),
                'v' => new('\v'),
                '0' => new('\0'),
                _ => throw new LexerException("Unrecognized escape sequence: \\" + escapeCharacter)
            };

            return result;
        }
        return rune;
    }
    
    public static Token ReadToken(
        ref this SpanReader<byte> reader,
        ref int line,
        ref int column,
        TokenInfo tokenInfo)
    {
        var originalPosition = reader.Position;
    beginLex:
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

        if (reader.StartsWith("//"u8))
        {
            var end = reader.Pending.IndexOf((byte)'\n');
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
            goto beginLex;
        }

        if (reader.StartsWith("/*"u8))
        {
            var remaining = reader.Pending[2..];
            var openCount = 1;
            while (0 < openCount)
            {
                var index = remaining.IndexOf((byte)'/');
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
            var newLineCount = commentBlock.Count((byte)'\n');
            if (0 < newLineCount)
            {
                line += newLineCount;
                column = 1;

                var index = commentBlock.LastIndexOf((byte)'\n') + 1;
                column += commentBlock.Length - index;
            }
            else
            {
                column += commentBlock.Length;
            }
            goto beginLex;
        }

        var result = new Token
        {
            Type = TokenType.IllegalCodePoint,
            Start = reader.Position,
            Line = line,
            Column = column
        };

        if (reader.WasConsumed)
        {
            result.Type = TokenType.Eof;
            return result;
        }

        if (first == ';')
        {
            result.Type = TokenType.Semicolon;
            result.Length = 1;
            ++reader.Position;
        }
        else if (first == '"')
        {
            result.Type = TokenType.LiteralString;
            var startPosition = reader.Position;
            ++reader.Position;
            while (true)
            {
                var rune = reader.ReadRune();
                if (rune.Value == '\n')
                {
                    // TODO: explode
                    ++line;
                    column = 1;
                }
                else
                {
                    ++column;
                }

                if (rune.Value == '"')
                    break;
                
                if (rune.Value == '\\')
                    _ = reader.ReadRune();
            }

            result.Length = reader.Position - startPosition;
        }
        else if (first == '\'')
        {
            result.Type = TokenType.LiteralCodePoint;
            ++reader.Position;
            var index = reader.Pending.IndexOf((byte)'\'');
            var end = index == -1 ? reader.Pending.Length : index + 1;
            reader.Position += end;
            result.Length = reader.Position - result.Start;
        }
        else if (first == '_' || Token.IsAlpha(first))
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
            if (tokenInfo.TokenTypeBySyntax.TryGetValue(identifier, out var tokenType))
                result.Type = tokenType;
        }
        else if (Token.IsDigit(first))
        {
            ++reader.Position;
            ++result.Length;
            ++column;
            result.Type = TokenType.LiteralInteger;
            while (Token.IsDigit(reader.PeekOrDefault()))
            {
                ++reader.Position;
                ++result.Length;
                ++column;
            }
        }
        else if (OpSymbols.Contains(first))
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
                foreach (var pair in tokenInfo.Utf8SymbolsOnly)
                {
                    var syntax = pair.Key.AsSpan();
                    if (matchLength < syntax.Length && pending.StartsWith(syntax))
                    {
                        matchLength = syntax.Length;
                        result.Type = pair.Value;
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

        // Safety measure to avoid infinite loops. :(
        if (reader.Position == originalPosition && !reader.WasConsumed)
            ++reader.Position;

        return result;
    }
    
    public static Rune ReadRune(ref this SpanReader<byte> reader)
    {
        var result = Rune.DecodeFromUtf8(
            reader.Pending,
            out var rune,
            out var bytesConsumed);
        if (result != OperationStatus.Done)
            throw new LexerException($"Error decoding UTF-8 code point ({result}).");
        reader.Position += bytesConsumed;
        return rune;
    }
}
