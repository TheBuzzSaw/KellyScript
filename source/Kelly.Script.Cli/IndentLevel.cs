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
        return builder.AppendRepeat(' ', indentLevel.FullWidth);
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

    public static StringBuilder AppendRepeat(
        this StringBuilder builder,
        char c,
        int count)
    {
        for (int i = 0; i < count; ++i)
            builder.Append(c);
        return builder;
    }
}