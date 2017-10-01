#include "SourceFile.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>

// Make sure these symbols are ascending order always!
static char theSymbols[] = "!#$%&()*+,-./:;<=>?@[\\]^`{|}~";
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
    
    char Current()
    {
        return source[index];
    }
    
    char Next()
    {
        return source[index + 1];
    }
    
    void Advance()
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
    
    void EatSpace()
    {
        while (index < length && IsSpace(Current())) Advance();
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
        
        while (index < length && IsIdentifierSafe(Current()))
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
        
        while (index < length && IsDigit(Current()))
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
        
        while (index < length)
        {
            ++lastSourceToken.length;
            
            if (Current() == '\\')
            {
                ++lastSourceToken.length;
                Advance();
                
                (void)IsEscapeSequence;
            }
            else if (Current() == '"')
            {
                Advance();
                break;
            }
            
            Advance();
        }
    }
    
    void ParseCharacterLiteral()
    {
        StartToken();
        lastSourceToken.token = Token::CharacterLiteral;
        Advance();
        
        while (index < length)
        {
            std::cout << "FOUND " << (int)Current() << '\n';
            ++lastSourceToken.length;
            
            if (Current() == '\\')
            {
                ++lastSourceToken.length;
                Advance();
                
                (void)IsEscapeSequence;
            }
            else if (Current() == '\'')
            {
                Advance();
                break;
            }
            
            Advance();
        }
    }
    
    void ParseLineComment()
    
    {
        lastSourceToken.token = Token::Comment;
        ++lastSourceToken.length;
        Advance();
        Advance();
        
        int line = position.line;
        
        while (index < length && line == position.line)
        {
            ++lastSourceToken.length;
            Advance();
        }
        
        if (line != position.line)
            --lastSourceToken.length;
    }
    
    void ParseBlockComment()
    {
        lastSourceToken.token = Token::Comment;
        ++lastSourceToken.length;
        Advance();
        Advance();
        
        int nestLevel = 1;
        
        while (index < length && nestLevel > 0)
        {
            if (Current() == '/' && Next() == '*')
            {
                ++nestLevel;
                ++lastSourceToken.length;
                Advance();
            }
            else if (Current() == '*' && Next() == '/')
            {
                --nestLevel;
                ++lastSourceToken.length;
                Advance();
            }
            
            ++lastSourceToken.length;
            Advance();
        }
    }
    
    void ParseSymbols()
    {
        StartToken();
        
        switch (Current())
        {
            case '/':
                if (Next() == '/')
                {
                    ParseLineComment();
                    break;
                }
                else if (Next() == '*')
                {
                    ParseBlockComment();
                    break;
                }
                
                // Yes. No break. This is deliberate.
                
            default:
                lastSourceToken.token = Token::Symbols;
                Advance();
                
                while (index < length && IsSymbol(Current()))
                {
                    ++lastSourceToken.length;
                    Advance();
                }
                break;
        }
    }
    
    bool Read()
    {
        bool result = false;
        
        EatSpace();
        if (index < length)
        {
            result = true;
            char c = Current();
            
            if (IsAlpha(c) || c == '_')
                ParseIdentifier();
            else if (IsDigit(c))
                ParseNumericLiteral();
            else if (c == '"')
                ParseStringLiteral();
            else if (c == '\'')
                ParseCharacterLiteral();
            else if (IsSymbol(c))
                ParseSymbols();
            else
                result = false;
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
    
    while (reader.Read())
        result.sourceTokens.push_back(reader.lastSourceToken);
    
    return result;
}

std::ostream& operator<<(std::ostream& stream, Token token)
{
    const char* text;
    
    switch (token)
    {
        case Token::Identifier: text = "identifier"; break;
        case Token::NumericLiteral: text = "numeric literal"; break;
        case Token::StringLiteral: text = "string literal"; break;
        case Token::CharacterLiteral: text = "character literal"; break;
        case Token::Symbols: text = "symbols"; break;
        case Token::Comment: text = "comment"; break;
        default: text = "unknown"; break;
    }
    
    return stream << text;
}

std::ostream& operator<<(std::ostream& stream, const SourceFile& sourceFile)
{
    for (const auto& sourceToken : sourceFile.sourceTokens)
    {
        stream << "Line " << sourceToken.textPosition.line
            << " Col " << sourceToken.textPosition.column
            << ' ' << sourceToken.token
            << " (" << sourceToken.length
            << ") : ";
        
        stream.write(
            sourceFile.source.data() + sourceToken.offset,
            sourceToken.length) << '\n';
    }
    
    return stream;
}

