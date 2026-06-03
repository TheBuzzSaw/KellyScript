using System.Text;

namespace Kelly.Script.Cli;

sealed class ImportStatement : SyntaxNode
{
    // Must be a single identifier or dot-separated identifier chain.
    public required SyntaxNode PackageName { get; init; }

    public override void Write(StringBuilder builder, IndentLevel indentLevel)
    {
        throw new System.NotImplementedException();
    }
}