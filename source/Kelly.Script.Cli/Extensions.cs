using System;
using System.Collections.Generic;
using System.Collections.Immutable;

namespace Kelly.Script.Cli;

static class Extensions
{
    public static ImmutableArray<T> OrEmpty<T>(this ImmutableArray<T> array) => array.IsDefault ? [] : array;
    public static void RemoveAt<T>(this List<T> list, Index index) => list.RemoveAt(index.GetOffset(list.Count));
}