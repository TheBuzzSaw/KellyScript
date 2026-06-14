using System;

namespace Kelly.Script.Cli;

sealed class LexerException : Exception
{
    public LexerException(
        string? message = null,
        Exception? innerException = null
        ) : base(message, innerException)
    {
    }
}