using System.Text;

namespace Kelly.Script.Cli;

abstract class SyntaxNode
{
    public abstract void Write(StringBuilder builder, IndentLevel indentLevel);
}