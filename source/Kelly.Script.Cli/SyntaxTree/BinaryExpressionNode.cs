namespace Kelly.Script.Cli;

abstract class BinaryExpressionNode : SyntaxNode
{
    public required SyntaxNode Left { get; init; }
    public required SyntaxNode Right { get; init; }
}