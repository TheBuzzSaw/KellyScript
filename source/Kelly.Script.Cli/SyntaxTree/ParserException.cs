using System;

namespace Kelly.Script.Cli;

sealed class ParserException : Exception
{
    public ParserException(
        string? message = null,
        Exception? innerException = null) : base(message, innerException)
    {
    }
}