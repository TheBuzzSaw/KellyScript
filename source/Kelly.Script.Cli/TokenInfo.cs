using System.Collections.Frozen;
using System.Collections.Generic;

namespace Kelly.Script.Cli;

sealed class TokenInfo
{
    public required FrozenDictionary<string, TokenType> TokenTypesByKeyword { get; init; }
    public static TokenInfo Create()
    {
        KeyValuePair<string, TokenType>[] pairs =
        [
            new("and", TokenType.KeywordAnd),
            new("as", TokenType.KeywordAs),
            new("base", TokenType.KeywordBase),
            new("bool", TokenType.KeywordBool),
            new("class", TokenType.KeywordClass),
            new("continue", TokenType.KeywordContinue),
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
            new("import", TokenType.KeywordImport),
            new("int8", TokenType.KeywordInt8),
            new("int16", TokenType.KeywordInt16),
            new("int32", TokenType.KeywordInt32),
            new("int64", TokenType.KeywordInt64),
            new("nand", TokenType.KeywordNand),
            new("nor", TokenType.KeywordNor),
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
            new("xor", TokenType.LogicalXor)
        ];

        var result = new TokenInfo
        {
            TokenTypesByKeyword = pairs.ToFrozenDictionary()
        };

        return result;
    }
}