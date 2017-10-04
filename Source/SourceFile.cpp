#include "SourceFile.hpp"
#include "TinyString.hpp"
#include "CString.hpp"
#include "DataMap.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>

static DataMap<String4, Token> theTokensBySource;
static DataMap<CString, Token> theTokensByKeyword;

// Make sure these symbols are ascending order always!
static char theSymbols[] = "!#$%&()*+,-./:;<=>?@[\\]^`{|}~";
static char theEscapeSequences[] = "\"'?\\abfnrtv";

static Token FindOperator(String4 source)
{
    auto token = theTokensBySource.TryGetValue(source);
    return token ? *token : Token::None;
}

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
        c == '\t' ||
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

static inline bool IsStringLiteralSafe(char c)
{
    return IsIdentifierSafe(c) || IsSymbol(c) || c == ' ';
}

struct SourceReader
{
    const char* source;
    int index;
    int length;
    TextPosition position;
    SourceToken lastSourceToken;
    TextPosition errorPosition;
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
        
        // Tokens never start with newline.
        ++index;
        ++position.column;
    }
    
    void ParseIdentifier()
    {
        StartToken();
        lastSourceToken.token = Token::Identifier;
        
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
        
        while (index < length)
        {
            ++lastSourceToken.length;
            
            if (Current() == '\\')
            {
                ++lastSourceToken.length;
                
                if (!IsEscapeSequence(Next()))
                {
                    errorPosition = lastSourceToken.textPosition;
                    errorMessage = "invalid escape sequence";
                    return;
                }
                
                Advance();
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
            
            Advance();
        }
    }
    
    void ParseCharacterLiteral()
    {
        StartToken();
        lastSourceToken.token = Token::CharacterLiteral;
        
        while (index < length)
        {
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
                lastSourceToken.token = FindOperator(tinySource);
                
                for (int i = index; i < length && IsSymbol(source[i]); ++i)
                {
                    tinySource.Append(source[i]);
                    auto token = FindOperator(tinySource);
                    
                    if (token != Token::None)
                    {
                        lastSourceToken.token = token;
                        lastSourceToken.length = i - index + 2;
                    }
                }
                
                index += lastSourceToken.length - 1;
                
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

static void MapOperatorToken(const char* source, Token token)
{
    String4 ts = {};
    ts.Assign(source);
    theTokensBySource.Set(ts, token);
}

static void MapKeywordToken(const char* keyword, Token token)
{
    CString cs = {keyword};
    theTokensByKeyword.Set(cs, token);
}

void PrepareLexer()
{
    MapOperatorToken(",", Token::Comma);
    MapOperatorToken("(", Token::OpenParen);
    MapOperatorToken(")", Token::CloseParen);
    MapOperatorToken("[", Token::OpenBracket);
    MapOperatorToken("]", Token::CloseBracket);
    MapOperatorToken("{", Token::OpenBrace);
    MapOperatorToken("}", Token::CloseBrace);
    MapOperatorToken("=", Token::AssignOp);
    MapOperatorToken("+=", Token::AddAssignOp);
    MapOperatorToken("-=", Token::SubAssignOp);
    MapOperatorToken("*=", Token::MultAssignOp);
    MapOperatorToken("/=", Token::DivAssignOp);
    MapOperatorToken("%=", Token::ModAssignOp);
    MapOperatorToken("&=", Token::AndAssignOp);
    MapOperatorToken("|=", Token::OrAssignOp);
    MapOperatorToken("^=", Token::XorAssignOp);
    MapOperatorToken("<<=", Token::LeftShiftAssignOp);
    MapOperatorToken(">>=", Token::RightShiftAssignOp);
    MapOperatorToken("+", Token::AddOp);
    MapOperatorToken("-", Token::SubOp);
    MapOperatorToken("*", Token::MultOp);
    MapOperatorToken("/", Token::DivOp);
    MapOperatorToken("%", Token::ModOp);
    MapOperatorToken("<<", Token::LeftShiftOp);
    MapOperatorToken(">>", Token::RightShiftOp);
    MapOperatorToken("++", Token::IncOp);
    MapOperatorToken("--", Token::DecOp);
    MapOperatorToken("!", Token::LogicalNotOp);
    MapOperatorToken("~", Token::BitwiseNotOp);
    MapOperatorToken("&&", Token::LogicalAndOp);
    MapOperatorToken("||", Token::LogicalOrOp);
    MapOperatorToken("&", Token::BitwiseAndOp);
    MapOperatorToken("|", Token::BitwiseOrOp);
    MapOperatorToken("^", Token::BitwiseXorOp);
    MapOperatorToken("==", Token::EqualOp);
    MapOperatorToken("!=", Token::NotEqualOp);
    MapOperatorToken("<", Token::LessOp);
    MapOperatorToken("<=", Token::LessOrEqualOp);
    MapOperatorToken(">", Token::GreaterOp);
    MapOperatorToken(">=", Token::GreaterOrEqualOp);
    MapOperatorToken(".", Token::Dot);
    MapOperatorToken(";", Token::Semicolon);
    MapOperatorToken(":", Token::Colon);
    MapOperatorToken("?", Token::Question);
    
    (void)MapKeywordToken;
}

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
    
    while (reader.Read() && reader.errorMessage.empty())
        result.sourceTokens.push_back(reader.lastSourceToken);
    
    if (!reader.errorMessage.empty())
    {
        std::cout << reader.errorPosition << " error - "
            << reader.errorMessage << '\n';
    }
    
    return result;
}

const char* TokenName(Token token)
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
        case Token::Comma: text = "comma"; break;
        case Token::OpenParen: text = "open parenthesis"; break;
        case Token::CloseParen: text = "close parenthesis"; break;
        case Token::OpenBracket: text = "open bracket"; break;
        case Token::CloseBracket: text = "close bracket"; break;
        case Token::OpenBrace: text = "open brace"; break;
        case Token::CloseBrace: text = "close brace"; break;
        case Token::AssignOp: text = "assignment operator"; break;
        case Token::AddAssignOp: text = "addition assignment operator"; break;
        case Token::SubAssignOp: text = "subtraction assignment operator"; break;
        case Token::MultAssignOp: text = "multiplication assignment operator"; break;
        case Token::DivAssignOp: text = "division assignment operator"; break;
        case Token::ModAssignOp: text = "remainder assignment operator"; break;
        case Token::AndAssignOp: text = "and assignment operator"; break;
        case Token::OrAssignOp: text = "or assignment operator"; break;
        case Token::XorAssignOp: text = "xor assignment operator"; break;
        case Token::LeftShiftAssignOp: text = "left shift assignment operator"; break;
        case Token::RightShiftAssignOp: text = "right shift assignment operator"; break;
        case Token::AddOp: text = "addition operator"; break;
        case Token::SubOp: text = "subtraction operator"; break;
        case Token::MultOp: text = "multiplication operator"; break;
        case Token::DivOp: text = "division operator"; break;
        case Token::ModOp: text = "remainder operator"; break;
        case Token::LeftShiftOp: text = "left shift operator"; break;
        case Token::RightShiftOp: text = "right shift operator"; break;
        case Token::IncOp: text = "increment operator"; break;
        case Token::DecOp: text = "decrement operator"; break;
        case Token::LogicalNotOp: text = "logical not operator"; break;
        case Token::BitwiseNotOp: text = "bitwise not operator"; break;
        case Token::LogicalAndOp: text = "logical and operator"; break;
        case Token::LogicalOrOp: text = "logical or operator"; break;
        case Token::BitwiseAndOp: text = "bitwise and operator"; break;
        case Token::BitwiseOrOp: text = "bitwise or operator"; break;
        case Token::BitwiseXorOp: text = "bitwise xor operator"; break;
        case Token::EqualOp: text = "equality operator"; break;
        case Token::NotEqualOp: text = "inequality operator"; break;
        case Token::LessOp: text = "less than operator"; break;
        case Token::LessOrEqualOp: text = "less than or equal to operator"; break;
        case Token::GreaterOp: text = "greater than operator"; break;
        case Token::GreaterOrEqualOp: text = "greater than or equal to operator"; break;
        case Token::Dot: text = "dot"; break;
        case Token::Semicolon: text = "semicolon"; break;
        case Token::Colon: text = "colon"; break;
        case Token::Question: text = "question mark"; break;
        default: text = "unknown"; break;
    }
    
    return text;
}

std::ostream& operator<<(std::ostream& stream, Token token)
{    
    return stream << TokenName(token);
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
            << ' ' << sourceToken.token
            << " (" << sourceToken.length
            << ") : ";
        
        stream.write(
            sourceFile.source.data() + sourceToken.offset,
            sourceToken.length) << '\n';
    }
    
    return stream;
}

