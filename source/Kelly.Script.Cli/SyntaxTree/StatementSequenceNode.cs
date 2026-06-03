using System.Collections.Immutable;

namespace Kelly.Script.Cli;

abstract class StatementSequenceNode : SyntaxNode
{
    private readonly ImmutableArray<SyntaxNode> _expressions;
    
    public ImmutableArray<SyntaxNode> Expressions
    {
        get => _expressions;
        init => _expressions = value.OrEmpty();
    }
}