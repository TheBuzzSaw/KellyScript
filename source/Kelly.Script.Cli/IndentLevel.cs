using System.Text;

namespace Kelly.Script.Cli;

readonly struct IndentLevel
{
    public int Level { get; init; }
    public int Width { get; init; }
    public int FullWidth => Level * Width;

    public IndentLevel(int level, int width)
    {
        Level = level;
        Width = width;
    }

    public IndentLevel Indent() => new(Level + 1, Width);
}

static class IndentLevelExtensions
{
    public static StringBuilder AppendIndent(
        this StringBuilder builder,
        IndentLevel indentLevel)
    {
        return builder.Append(' ', indentLevel.FullWidth);
    }

    public static StringBuilder AppendTreeBranch(
        this StringBuilder builder,
        IndentLevel indentLevel)
    {
        builder.AppendIndent(indentLevel);
        if (0 < indentLevel.Level)
            builder.Append('└');
        return builder;
    }
}