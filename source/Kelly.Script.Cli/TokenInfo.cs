using System.Collections.Frozen;
using System.Collections.Generic;
using System.Collections.Immutable;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;

namespace Kelly.Script.Cli;

sealed class TokenInfo
{
    public ImmutableArray<KeyValuePair<string, TokenType>> SymbolsOnly { get; init; } = [];
    public ImmutableArray<KeyValuePair<ImmutableArray<byte>, TokenType>> Utf8SymbolsOnly { get; init; } = [];
    public required FrozenDictionary<string, TokenType> TokenTypeBySyntax { get; init; }
    public required FrozenDictionary<TokenType, string> SyntaxByTokenType { get; init; }
    public static TokenInfo Create()
    {
        KeyValuePair<string, TokenType>[] pairs =
        [
            new("and", TokenType.KeywordAnd),
            new("as", TokenType.KeywordAs),
            new("base", TokenType.KeywordBase),
            new("bool", TokenType.KeywordBool),
            new("break", TokenType.KeywordBreak),
            new("class", TokenType.KeywordClass),
            new("continue", TokenType.KeywordContinue),
            new("defer", TokenType.KeywordDefer),
            new("do", TokenType.KeywordDo),
            new("else", TokenType.KeywordElse),
            new("export", TokenType.KeywordExport),
            new("false", TokenType.KeywordFalse),
            new("float16", TokenType.KeywordFloat16),
            new("float32", TokenType.KeywordFloat32),
            new("float64", TokenType.KeywordFloat64),
            new("float128", TokenType.KeywordFloat128),
            new("float256", TokenType.KeywordFloat256),
            new("for", TokenType.KeywordFor),
            new("if", TokenType.KeywordIf),
            new("in", TokenType.KeywordIn),
            new("is", TokenType.KeywordIs),
            new("import", TokenType.KeywordImport),
            new("int8", TokenType.KeywordInt8),
            new("int16", TokenType.KeywordInt16),
            new("int32", TokenType.KeywordInt32),
            new("int64", TokenType.KeywordInt64),
            new("nand", TokenType.KeywordNand),
            new("nor", TokenType.KeywordNor),
            new("of", TokenType.KeywordOf),
            new("or", TokenType.KeywordOr),
            new("property", TokenType.KeywordProperty),
            new("size", TokenType.KeywordSize),
            new("struct", TokenType.KeywordStruct),
            new("to", TokenType.KeywordTo),
            new("true", TokenType.KeywordTrue),
            new("type", TokenType.KeywordType),
            new("uint8", TokenType.KeywordUInt8),
            new("uint16", TokenType.KeywordUInt16),
            new("uint32", TokenType.KeywordUInt32),
            new("uint64", TokenType.KeywordUInt64),
            new("usize", TokenType.KeywordUSize),
            new("while", TokenType.KeywordWhile),
            new("xnor", TokenType.KeywordXnor),
            new("xor", TokenType.KeywordXor),
            new("(", TokenType.OpenParen),
            new(")", TokenType.CloseParen),
            new("[", TokenType.OpenBracket),
            new("]", TokenType.CloseBracket),
            new("{", TokenType.OpenBrace),
            new("}", TokenType.CloseBrace),
            new(".", TokenType.Dot),
            new(",", TokenType.Comma),
            new("*", TokenType.Splat),
            new("/", TokenType.Slash),
            new("%", TokenType.Percent),
            new("+", TokenType.Plus),
            new("-", TokenType.Minus),
            new("<<", TokenType.ShiftLeft),
            new(">>", TokenType.ShiftRight),
            new("<", TokenType.LessThan),
            new("<=", TokenType.LessThanOrEqual),
            new(">", TokenType.GreaterThan),
            new(">=", TokenType.GreaterThanOrEqual),
            new("==", TokenType.Equal),
            new("!=", TokenType.NotEqual),
            new("&", TokenType.BitwiseAnd),
            new("^", TokenType.BitwiseXor),
            new("|", TokenType.BitwiseOr),
            new("=", TokenType.Assign),
            new("+=", TokenType.AssignSum),
            new("-=", TokenType.AssignDiff),
            new("*=", TokenType.AssignProduct),
            new("/=", TokenType.AssignQuotient),
            new("%=", TokenType.AssignMod),
            new("<<=", TokenType.AssignLeftShift),
            new(">>=", TokenType.AssignRightShift),
            new("&=", TokenType.AssignAnd),
            new("^=", TokenType.AssignXor),
            new("|=", TokenType.AssignOr),
            new(";", TokenType.Semicolon),
            new("!", TokenType.Bang)
        ];

        var symbolsOnly = pairs.Where(IsSymbols);

        var result = new TokenInfo
        {
            SymbolsOnly = symbolsOnly.ToImmutableArray(),
            Utf8SymbolsOnly = symbolsOnly.Select(ToUtf8).ToImmutableArray(),
            TokenTypeBySyntax = pairs.ToFrozenDictionary(),
            SyntaxByTokenType = pairs.Select(Swap).ToFrozenDictionary()
        };

        return result;

        static bool IsSymbols(KeyValuePair<string, TokenType> pair) =>
            !Token.IsAlpha(pair.Key[0]);

        static KeyValuePair<ImmutableArray<byte>, TokenType> ToUtf8(
            KeyValuePair<string, TokenType> pair)
        {
            var utf8 = Encoding.UTF8.GetBytes(pair.Key);
            var array = ImmutableCollectionsMarshal.AsImmutableArray(utf8);
            return new(array, pair.Value);
        }

        static KeyValuePair<TValue, TKey> Swap<TKey, TValue>(
            KeyValuePair<TKey, TValue> pair) =>
                new(pair.Value, pair.Key);
    }
}