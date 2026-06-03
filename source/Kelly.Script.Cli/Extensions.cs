using System.Collections.Immutable;

namespace Kelly.Script.Cli;

static class Extensions
{
    public static ImmutableArray<T> OrEmpty<T>(this ImmutableArray<T> array) => array.IsDefault ? ImmutableArray<T>.Empty : array;
}