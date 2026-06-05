namespace Kelly.Script.Cli;

sealed class BinaryOperatorInfo
{
    public string Syntax { get; }
    public TokenType Token { get; }
    public int Precedence { get; }

    public BinaryOperatorInfo(
        string syntax,
        TokenType tokenType,
        int precedence)
    {
        Syntax = syntax;
        Token = tokenType;
        Precedence = precedence;
    }
}