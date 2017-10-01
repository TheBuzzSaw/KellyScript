#include "SourceFile.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

// Make sure these symbols are ascending order always!
static char theSymbols[] = "!\"#$%&'()*+,-./:;<=>?@[\\]^`{|}~";
static char theEscapeSequences[] = "\"'?\\abfnrtv";

static bool IsSymbol(char c)
{
    return std::binary_search(
        theSymbols,
        theSymbols + sizeof(theSymbols) - 1,
        c);
}

static bool IsEscapeSequence(char c)
{
    return std::binary_search(
        theEscapeSequences,
        theEscapeSequences + sizeof(theEscapeSequences) - 1,
        c);
}

static std::string FileToString(const char* file)
{
    std::ifstream fin(file, std::ifstream::binary);
    
    if (fin)
    {
        std::ostringstream oss;
        oss << fin.rdbuf();
        fin.close();
        return oss.str();
    }
    
    return std::string();
}

static constexpr bool IsSpace(char c)
{
    return
        c == ' ' ||
        c == '\r' ||
        c == '\n';
}

static constexpr bool IsDigit(char c)
{
    return '0' <= c && c <= '9';
}

static constexpr bool IsUpper(char c)
{
    return 'A' <= c && c <= 'Z';
}

static constexpr bool IsLower(char c)
{
    return 'a' <= c && c <= 'z';
}

static constexpr bool IsAlpha(char c)
{
    return IsUpper(c) || IsLower(c);
}

static constexpr bool IsIdentifierSafe(char c)
{
    return IsAlpha(c) || IsDigit(c) || c == '_';
}

struct SourceReader
{
    const char* source;
    int index;
    int length;
    TextPosition position;
    SourceToken lastSourceToken;
    std::string errorMessage;
    
    void Advance()
    {
        ++position.column;
        ++index;
    }
    
    void EatSpace()
    {
        while (index < length && IsSpace(source[index]))
        {
            if (source[index++] == '\n')
            {
                ++position.line;
                position.column = 1;
            }
            else
            {
                ++position.column;
            }
        }
    }
    
    void StartToken()
    {
        lastSourceToken.offset = index;
        lastSourceToken.length = 1;
        lastSourceToken.textPosition = position;
    }
    
    void ParseIdentifier()
    {
        StartToken();
        lastSourceToken.token = Token::Identifier;
        Advance();
        
        while (index < length && IsIdentifierSafe(source[index]))
        {
            ++lastSourceToken.length;
            Advance();
        }
    }
    
    void ParseNumericLiteral()
    {
        StartToken();
        lastSourceToken.token = Token::NumericLiteral;
        Advance();
        
        while (index < length && IsDigit(source[index]))
        {
            ++lastSourceToken.length;
            Advance();
        }
    }
    
    void ParseStringLiteral()
    {
        StartToken();
        lastSourceToken.token = Token::StringLiteral;
        Advance();
        
        while (index < length && source[index] != '"')
        {
            ++lastSourceToken.length;
            
            if (source[index] == '\\')
            {
                ++lastSourceToken.length;
                Advance();
                
                (void)IsEscapeSequence;
            }
            
            Advance();
        }
    }
    
    void ParseSymbols()
    {
        StartToken();
        lastSourceToken.token = Token::Symbols;
        Advance();
        
        while (index < length && IsSymbol(source[index]))
        {
            ++lastSourceToken.length;
            Advance();
        }
    }
    
    bool Read()
    {
        bool result = false;
        
        EatSpace();
        if (index < length)
        {
            char c = source[index];
            
            if (IsAlpha(c) || c == '_')
                ParseIdentifier();
            else if (IsDigit(c))
                ParseNumericLiteral();
            else if (c == '"')
                ParseStringLiteral();
            else if (IsSymbol(c))
                ParseSymbols();
        }
        
        return result;
    }
};

SourceFile LexSource(const char* file)
{
    SourceFile result;
    result.file = file;
    result.source = FileToString(file);
    //std::cout << file << '\n' << _sourceCode << '\n';
    
    SourceReader reader;
    reader.source = result.source.data();
    reader.index = 0;
    reader.length = (int)result.source.size();
    reader.position = {1, 1};
    
    std::sort(theSymbols, theSymbols + sizeof(theSymbols) - 1);
    std::cout << theSymbols << '\n';
    
    std::sort(
        theEscapeSequences,
        theEscapeSequences + sizeof(theEscapeSequences) - 1);
    std::cout << theEscapeSequences << '\n';
    
    return result;
}

