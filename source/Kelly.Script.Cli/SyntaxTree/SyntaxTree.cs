using System;
using System.IO;
using System.Text;

namespace Kelly.Script.Cli;

sealed class SyntaxTree : SyntaxNode
{
    private static readonly Encoding theEncoding = new UTF8Encoding(false);
    private static ReadOnlySpan<char> Whitespace => " \r\t";
    
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
        var sourceCode = File.ReadAllText(sourceFile, theEncoding);
        var reader = SpanReader.Create(sourceCode);
        int line = 1;
        int column = 1;

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
        }

        var sourceCodeUtf8 = File.ReadAllBytes(sourceFile);
        var tokens = TokenExtensions.Lex(sourceCodeUtf8, tokenInfo);

        var result = new SyntaxTree
        {
            SourceFile = sourceFile
        };

        return result;
    }
}