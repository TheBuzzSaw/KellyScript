using System.Collections.Immutable;

namespace Kelly.Script.Cli;

sealed class SyntaxForest
{
    public ImmutableArray<SyntaxTree> SyntaxTrees { get; init; } = [];
}