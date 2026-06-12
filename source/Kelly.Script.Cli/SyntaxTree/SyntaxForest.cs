using System.Collections.Immutable;

namespace Kelly.Script.Cli;

sealed class SyntaxForest
{
    private readonly ImmutableArray<SyntaxTree> _syntaxTrees;
    public required ImmutableArray<SyntaxTree> SyntaxTrees
    {
        get => _syntaxTrees;
        init => _syntaxTrees = value.OrEmpty();
    }
}