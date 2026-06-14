using System;
using System.Collections.Immutable;

namespace Kelly.Script.Cli;

sealed class SyntaxForest
{
    public ImmutableArray<SyntaxTree> SyntaxTrees { get; init; } = [];

    public static SyntaxForest FromProjectFile(string projectFile)
    {
        throw new NotImplementedException();
    }
}