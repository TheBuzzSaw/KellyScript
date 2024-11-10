using System;

namespace Kelly.Script.Cli;

public ref struct SpanReader<T>
{
    public ReadOnlySpan<T> Span;
    public int Position;

    public readonly bool HasMore => Position < Span.Length;
    public readonly ReadOnlySpan<T> Consumed => Span[..Position];
    public readonly ReadOnlySpan<T> Remaining => Span[Position..];
}

public static class SpanReader
{
    public static SpanReader<T> ToReader<T>(this Span<T> span) => new() { Span = span };
    public static SpanReader<T> ToReader<T>(this T[]? array) => new() { Span = array };
    public static T Peek<T>(ref this SpanReader<T> reader) => reader.Span[reader.Position];
    public static T Chomp<T>(ref this SpanReader<T> reader)
    {
        var result = reader.Span[reader.Position];
        ++reader.Position;
        return result;
    }
}
