#include "SourceFile.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>

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
        c == '\t' ||
        c == '\n';
}

static constexpr bool IsDigit(char c)
{
    return '0' <= c && c <= '9';
}

static constexpr bool IsUpperHex(char c)
{
    return 'A' <= c && c <= 'F';
}

static constexpr bool IsLowerHex(char c)
{
    return 'a' <= c && c <= 'f';
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

static inline bool IsStringLiteralSafe(char c)
{
    return IsIdentifierSafe(c) || IsSymbol(c) || c == ' ' || c == '\'';
}

static inline int ParseHex(char c)
{
    if (IsDigit(c))
        return c - '0';
    else if (IsUpperHex(c))
        return c - 'A' + 10;
    else if (IsLowerHex(c))
        return c - 'a' + 10;
    
    return -1;
}

struct SourceReader
{
    const char* source;
    int index = 0;
    int length = 0;
    TextPosition position = {};
    SourceToken lastSourceToken = {};
    std::string buffer;
    Literal literal;
    TextPosition errorPosition = {};
    std::string errorMessage;
    
    char Current()
    {
        return source[index];
    }
    
    char Next()
    {
        return source[index + 1];
    }
    
    char Previous()
    {
        return source[index - 1];
    }
    
    int Gap()
    {
        return length - index;
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
        lastSourceToken.tokenIndex = -1;
        
        // Tokens never start with newline.
        ++index;
        ++position.column;
    }
    
    void ParseIdentifier()
    {
        StartToken();
        lastSourceToken.tokenType = TokenType::Identifier;
        
        while (index < length && IsIdentifierSafe(Current()))
        {
            ++lastSourceToken.length;
            Advance();
        }
        
        buffer.assign(source + lastSourceToken.offset, lastSourceToken.length);
        auto tokenIndex = FindKeyword(buffer.data());

        if (tokenIndex != -1)
        {
            lastSourceToken.tokenType = TokenType::Keyword;
            lastSourceToken.tokenIndex = tokenIndex;
        }
    }

    void ParseFloatLiteral()
    {
        lastSourceToken.tokenType = TokenType::Float64Literal;

        while (index < length)
        {
            if (IsDigit(Current()))
            {
                ++lastSourceToken.length;
                buffer += Current();
                Advance();
            }
            else
            {
                break;
            }
        }

        literal.asFloat64 = stod(buffer);
    }
    
    void ParseIntegerLiteral()
    {
        literal.asUInt64 = Current() - '0';
        StartToken();
        lastSourceToken.tokenType = TokenType::UnsignedIntegerLiteral;
        
        while (index < length)
        {
            if (IsDigit(Current()))
            {
                ++lastSourceToken.length;
                literal.asUInt64 = literal.asUInt64 * 10 + (Current() - '0');
                Advance();
            }
            else if (Current() == '.')
            {
                buffer.assign(source + lastSourceToken.offset, lastSourceToken.length++);
                Advance();
                ParseFloatLiteral();
                break;
            }
            else
            {
                break;
            }
        }
    }
    
    void ParseHexEscapeSequence()
    {
        if (Gap() > 1)
        {
            int hex = ParseHex(Next());
            
            if (hex >= 0)
            {
                Advance();
                ++lastSourceToken.length;
                
                int total = hex;
                hex = ParseHex(Next());
                
                while (hex >= 0 && Gap() > 1)
                {
                    Advance();
                    ++lastSourceToken.length;
                    
                    total = total * 0x10 + hex;
                    hex = ParseHex(Next());
                }
                
                if (total > 255)
                {
                    errorPosition = lastSourceToken.textPosition;
                    errorMessage = "escape sequence hex value out of range";
                }
                else
                {
                    buffer += (char)total;
                }
            }
            else
            {
                errorPosition = lastSourceToken.textPosition;
                errorMessage = "expected at least one hex digit (";
                errorMessage += Next();
                errorMessage += ") [";
                errorMessage += buffer;
                errorMessage += "]";
            }
        }
        else
        {
            errorPosition = lastSourceToken.textPosition;
            errorMessage = "found EOF; expected hex value";
        }
    }
    
    void ParseEscapeSequence()
    {
        ++index;
        ++position.column;
        
        if (index < length)
        {
            ++lastSourceToken.length;
            
            switch (Current())
            {
                case '\'': buffer += '\''; break;
                case '"': buffer += '"'; break;
                case '?': buffer += '\?'; break;
                case '\\': buffer += '\\'; break;
                case 'a': buffer += '\a'; break;
                case 'b': buffer += '\b'; break;
                case 'f': buffer += '\f'; break;
                case 'n': buffer += '\n'; break;
                case 'r': buffer += '\r'; break;
                case 't': buffer += '\t'; break;
                case 'v': buffer += '\v'; break;
                case 'x': ParseHexEscapeSequence(); break;
                default:
                    errorPosition = lastSourceToken.textPosition;
                    errorMessage = "invalid escape sequence";
                    break;
            }
            
            Advance();
        }
        else
        {
            errorPosition = lastSourceToken.textPosition;
            errorMessage = "found EOF; expected escape sequence";
        }
    }
    
    void ParseStringLiteral()
    {
        StartToken();
        lastSourceToken.tokenType = TokenType::StringLiteral;
        buffer.clear();
        
        while (index < length && errorMessage.empty())
        {
            ++lastSourceToken.length;
            
            if (Current() == '\\')
            {
                ParseEscapeSequence();
                continue;
            }
            else if (Current() == '"')
            {
                Advance();
                break;
            }
            else if (!IsStringLiteralSafe(Current()))
            {
                errorPosition = lastSourceToken.textPosition;
                errorMessage = "invalid character; expected \"";
                return;
            }
            else
            {
                buffer += Current();
            }
            
            Advance();
        }
    }
    
    void ParseCodePointLiteral()
    {
        StartToken();
        lastSourceToken.tokenType = TokenType::CodePointLiteral;
        buffer.clear();
        
        while (index < length && errorMessage.empty())
        {
            ++lastSourceToken.length;

            if (Current() == '\'')
            {
                if (buffer.size() > 1)
                {
                    errorMessage = "character literal may not contain multiple characters";
                    errorPosition = lastSourceToken.textPosition;
                }

                Advance();
                break;
            }
            else if (Current() == '\\')
            {
                ParseEscapeSequence();
                continue;
            }
            else
            {
                buffer += Current();
                Advance();
            }
        }

        if (buffer.size() < 1)
            literal.asCodePoint = 0;
        else
            literal.asCodePoint = buffer[0];
    }
    
    void ParseLineComment()
    {
        lastSourceToken.tokenType = TokenType::Comment;
        ++lastSourceToken.length;
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
        lastSourceToken.tokenType = TokenType::Comment;
        ++lastSourceToken.length;
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
        
        switch (Previous())
        {
            case '/':
                if (Current() == '/')
                {
                    ParseLineComment();
                    break;
                }
                else if (Current() == '*')
                {
                    ParseBlockComment();
                    break;
                }
                
                // Yes. No break. This is deliberate.
                
            default:
            {
                String4 tinySource = {};
                tinySource.Append(Previous());
                lastSourceToken.tokenType = TokenType::Operator;
                lastSourceToken.tokenIndex = FindOperator(tinySource);
                
                for (int i = index; i < length && IsSymbol(source[i]); ++i)
                {
                    tinySource.Append(source[i]);
                    auto tokenIndex = FindOperator(tinySource);
                    
                    if (tokenIndex != -1)
                    {
                        lastSourceToken.tokenIndex = tokenIndex;
                        lastSourceToken.length = i - index + 2;
                    }
                }
                
                int offset = lastSourceToken.length - 1;
                index += offset;
                position.column += offset;
                
                break;
            }
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
                ParseIntegerLiteral();
            else if (c == '"')
                ParseStringLiteral();
            else if (c == '\'')
                ParseCodePointLiteral();
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
    (void)IsEscapeSequence;
    SourceFile result;
    result.file = file;
    result.source = FileToString(file);
    //std::cout << file << '\n' << _sourceCode << '\n';
    
    SourceReader reader;
    reader.source = result.source.data();
    reader.index = 0;
    reader.length = (int)result.source.size();
    reader.position = {1, 1};
    
    while (reader.Read() && reader.errorMessage.empty())
    {
        switch (reader.lastSourceToken.tokenType)
        {
            case TokenType::StringLiteral:
                reader.lastSourceToken.tokenIndex = result.strings.size();
                result.strings.push_back(reader.buffer);
                break;
            case TokenType::CodePointLiteral:
            case TokenType::Float32Literal:
            case TokenType::Float64Literal:
            case TokenType::SignedIntegerLiteral:
            case TokenType::UnsignedIntegerLiteral:
                reader.lastSourceToken.tokenIndex = result.literals.size();
                result.literals.push_back(reader.literal);
                break;
            default: break;
        }
        
        result.sourceTokens.push_back(reader.lastSourceToken);
    }
    
    if (!reader.errorMessage.empty())
    {
        std::cout << reader.errorPosition << " error - "
            << reader.errorMessage << '\n';
    }
    
    return result;
}

std::ostream& operator<<(std::ostream& stream, TextPosition position)
{
    return stream << "Line " << position.line << " Col " << position.column;
}

std::ostream& operator<<(std::ostream& stream, const SourceFile& sourceFile)
{
    for (const auto& sourceToken : sourceFile.sourceTokens)
    {
        stream << sourceToken.textPosition
            << ' ' << sourceToken.tokenType;
        
        if (sourceToken.tokenType == TokenType::Keyword)
        {
            stream
                << " ("
                << KeywordToken(sourceToken.tokenIndex)->description
                << ")";
        }
        else if (sourceToken.tokenType == TokenType::Operator)
        {
            stream
                << " ("
                << OperatorToken(sourceToken.tokenIndex)->description
                << ")";
        }
        
        stream << " (" << sourceToken.length << ") : ";
        
        stream.write(
            sourceFile.source.data() + sourceToken.offset,
            sourceToken.length) << '\n';
    }
    
    return stream;
}

