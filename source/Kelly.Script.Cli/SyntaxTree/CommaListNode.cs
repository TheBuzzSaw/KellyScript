using System.Collections.Immutable;
using System.Text;

namespace Kelly.Script.Cli;

sealed class CommaListNode : SyntaxNode
{
    public ImmutableArray<SyntaxNode> Nodes { get; init; } = [];

    public override void Write(StringBuilder builder, IndentLevel indentLevel)
    {
        throw new System.NotImplementedException();
    }
}