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
        var tokens = TokenExtensions.Lex(sourceCodeUtf8, tokenInfo);
        var reader = SpanReader.Create(tokens);

        var result = new SyntaxTree
        {
            SourceFile = sourceFile
        };

        return result;
    }
}