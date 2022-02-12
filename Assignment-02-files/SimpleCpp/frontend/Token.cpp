/**
 * Token class for a simple interpreter.
 *
 * (c) 2020 by Ronald Mak
 * Department of Computer Science
 * San Jose State University
 */
#include <string>
#include <map>
#include <ctype.h>

#include "../Object.h"
#include "Token.h"

namespace frontend {

using namespace std;

map<string, TokenType> Token::reservedWords;

void Token::initialize()
{
    reservedWords["PROGRAM"] = PROGRAM;
    reservedWords["BEGIN"]   = BEGIN;
    reservedWords["END"]     = END;
    reservedWords["REPEAT"]  = REPEAT;
    reservedWords["UNTIL"]   = UNTIL;
    reservedWords["WRITE"]   = WRITE;
    reservedWords["WRITELN"] = WRITELN;
}

Token *Token::Word(char firstChar, Source *source)
{
    Token *token = new Token(firstChar);
    token->lineNumber = source->lineNumber();

    // Loop to get the rest of the characters of the word token.
    // Append letters and digits to the token.
    for (char ch = source->nextChar(); isalnum(ch); ch = source->nextChar())
    {
        token->text += ch;
    }

    // Is it a reserved word or an identifier?
    string upper = toUpperCase(token->text);
    if (Token::reservedWords.find(upper) != Token::reservedWords.end())
    {
        token->type = Token::reservedWords[upper];
    }
    else
    {
        token->type = TokenType::IDENTIFIER;
    }

    return token;
}

Token *Token::Number(char firstChar, Source *source)
{
    Token *token = new Token(firstChar);
    int pointCount = 0;

    // Loop to get the rest of the characters of the number token.
    // Append digits to the token.
    for (char ch = source->nextChar();
         isdigit(ch) || (ch == '.');
         ch = source->nextChar())
    {
        if (ch == '.') pointCount++;
        token->text += ch;
    }

    // Integer constant.
    if (pointCount == 0)
    {
        token->type    = TokenType::INTEGER;
        token->value.L = stol(token->text);
        token->value.D = token->value.L;  // allow using integer value as double
    }

    // Real constant.
    else if (pointCount == 1)
    {
        token->type    = TokenType::REAL;
        token->value.D = stod(token->text);
    }

    else tokenError(token, "Invalid number");

    return token;
}

Token *Token::String(char firstChar, Source *source)
{
    Token *token = new Token(firstChar);  // the leading '

    // Loop to append the rest of the characters of the string,
    // up to but not including the closing quote.
    for (char ch = source->nextChar(); ch != '\''; ch = source->nextChar())
    {
        token->text += ch;
    }

    token->text += '\'';  // the closing quote
    source->nextChar();  // consume the closing quote

    token->type = TokenType::STRING;

    // Don't include the leading and trailing '.
    token->value.S = token->text.substr(1, token->text.length() - 2);

    return token;
}

Token *Token::SpecialSymbol(char firstChar, Source *source)
{
    Token *token = new Token(firstChar);

    switch (firstChar)
    {
        case '.' : token->type = TokenType::PERIOD;     break;
        case ';' : token->type = TokenType::SEMICOLON;  break;
        case '+' : token->type = TokenType::PLUS;       break;
        case '-' : token->type = TokenType::MINUS;      break;
        case '*' : token->type = TokenType::STAR;       break;
        case '/' : token->type = TokenType::SLASH;      break;
        case '=' : token->type = TokenType::EQUALS;     break;
        case '<' : token->type = TokenType::LESS_THAN;  break;
        case '(' : token->type = TokenType::LPAREN;     break;
        case ')' : token->type = TokenType::RPAREN;     break;

        case ':' :
        {
            char nextChar = source->nextChar();
            token->text += nextChar;

            // Is it the := symbol?
            if (nextChar == '=')
            {
                token->type = TokenType::COLON_EQUALS;
            }

            // No, it's just the : symbol.
            else
            {
                token->type = TokenType::COLON;
                return token;  // already consumed :
            }

            break;
        }

        case EOF : token->type = END_OF_FILE; break;

        default: token->type = TokenType::ERROR;
    }

    source->nextChar();  // consume the special symbol
    return token;
}

void Token::tokenError(Token *token, string message)
{
    printf("TOKEN ERROR at line %d: %s at '%s'\n",
           token->lineNumber, message.c_str(), token->text.c_str());
}

}  // namespace frontend
