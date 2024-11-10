using System;
using System.IO;
using System.Text;

namespace Kelly.Script.Cli;

internal class Program
{
    private static void Main(string[] args)
    {
        try
        {
            var path = args[0];
            Console.WriteLine($"Tokenizing {path}");
            var code = File.ReadAllBytes(path);
            var reader = code.ToReader();
            int line = 1;
            int column = 1;
            while (true)
            {
                reader.ParseToken(ref line, ref column, out var token);
                if (token.Type == TokenType.Eof)
                    break;
                if (token.Type != TokenType.Gap)
                {
                    var text = Encoding.UTF8.GetString(code.AsSpan(token.Start, token.Length));
                    Console.WriteLine($"Line {token.Line} Col {token.Column} ({token.Type}): {text}");
                }
            }
        }
        catch (Exception ex)
        {
            Console.ResetColor();
            Console.ForegroundColor = ConsoleColor.Red;
            Console.WriteLine();
            Console.WriteLine(ex);
            Console.WriteLine();
            Console.ResetColor();
        }
    }
}
