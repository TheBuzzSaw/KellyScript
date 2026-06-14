using System;
using System.Collections.Generic;
using System.Collections.Immutable;
using System.IO;
using System.Text.Json;

namespace Kelly.Script.Cli;

sealed class SyntaxForest
{
    public string? ProjectFile { get; init; }
    public string? BasePackage { get; init; }
    public ImmutableArray<SyntaxTree> SyntaxTrees { get; init; } = [];

    public static SyntaxForest FromProjectFolder(string projectFolder)
    {
        var projectFile = default(string?);
        foreach (var file in Directory.EnumerateFiles(projectFolder, "*.json"))
        {
            if (projectFile is not null)
                throw new InvalidOperationException("Ambiguous project files.");
            projectFile = file;
        }

        if (projectFile is null)
            throw new InvalidOperationException("Unable to locate project file.");

        return FromProjectFile(projectFile);
    }

    public static SyntaxForest FromProjectFile(string projectFile)
    {
        ProjectSettings? projectSettings;
        {
            using var stream = File.OpenRead(projectFile);
            projectSettings = JsonSerializer.Deserialize<ProjectSettings>(stream);
        }

        var builder = ImmutableArray.CreateBuilder<SyntaxTree>();
        var baseFolder = Path.GetDirectoryName(projectFile) ?? "";
        var pendingFolders = new Stack<string>();
        pendingFolders.Push(baseFolder);
        var operatorIndex = OperatorIndex.Create();
        var tokenInfo = TokenInfo.Create();
        while (pendingFolders.TryPop(out var folder))
        {
            foreach (var nextFolder in Directory.EnumerateDirectories(folder))
                pendingFolders.Push(nextFolder);
            foreach (var sourceFile in Directory.EnumerateFiles(folder, "*.k"))
            {
                var syntaxTree = SyntaxTree.FromFile(sourceFile, operatorIndex, tokenInfo);
                builder.Add(syntaxTree);
            }
        }

        var result = new SyntaxForest
        {
            ProjectFile = projectFile,
            BasePackage = projectSettings?.BasePackage,
            SyntaxTrees = builder.DrainToImmutable()
        };

        return result;
    }
}