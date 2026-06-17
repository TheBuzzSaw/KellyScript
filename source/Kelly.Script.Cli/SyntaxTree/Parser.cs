namespace Kelly.Script.Cli;

sealed class Parser
{
    private Stage _stage = Stage.Import;

    private enum Stage
    {
        None,
        Import,
        Package,
        Code
    }
}