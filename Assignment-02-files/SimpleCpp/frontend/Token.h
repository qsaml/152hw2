/**
 * Token class for a simple interpreter.
 *
 * (c) 2020 by Ronald Mak
 * Department of Computer Science
 * San Jose State University
 */
#ifndef TOKEN_H_
#define TOKEN_H_

#include <string>
#include <map>

#include "../Object.h"
#include "Source.h"

namespace frontend {

using namespace std;

enum class TokenType
{
    PROGRAM, BEGIN, END, REPEAT, UNTIL, WRITE, WRITELN,
    PERIOD, COLON, COLON_EQUALS, SEMICOLON,
    PLUS, MINUS, STAR, SLASH, LPAREN, RPAREN,
    EQUALS, LESS_THAN,
    IDENTIFIER, INTEGER, REAL, STRING, END_OF_FILE, ERROR
};

static const string TOKEN_TYPE_STRINGS[] =
{
    "PROGRAM", "BEGIN", "END", "REPEAT", "UNTIL", "WRITE", "WRITELN",
    "PERIOD", "COLON", "COLON_EQUALS", "SEMICOLON",
    "PLUS", "MINUS", "STAR", "SLASH", "LPAREN", "RPAREN",
    "EQUALS", "LESS_THAN",
    "IDENTIFIER", "INTEGER", "REAL", "STRING", "EOF", "ERROR"
};

constexpr TokenType PROGRAM      = TokenType::PROGRAM;
constexpr TokenType BEGIN        = TokenType::BEGIN;
constexpr TokenType END          = TokenType::END;
constexpr TokenType REPEAT       = TokenType::REPEAT;
constexpr TokenType UNTIL        = TokenType::UNTIL;
constexpr TokenType WRITE        = TokenType::WRITE;
constexpr TokenType WRITELN      = TokenType::WRITELN;

constexpr TokenType PERIOD       = TokenType::PERIOD;
constexpr TokenType COLON        = TokenType::COLON;
constexpr TokenType COLON_EQUALS = TokenType::COLON_EQUALS;
constexpr TokenType SEMICOLON    = TokenType::SEMICOLON;
constexpr TokenType PLUS         = TokenType::PLUS;
constexpr TokenType MINUS        = TokenType::MINUS;
constexpr TokenType STAR         = TokenType::STAR;
constexpr TokenType SLASH        = TokenType::SLASH;
constexpr TokenType LPAREN       = TokenType::LPAREN;
constexpr TokenType RPAREN       = TokenType::RPAREN;
constexpr TokenType EQUALS       = TokenType::EQUALS;
constexpr TokenType LESS_THAN    = TokenType::LESS_THAN;
constexpr TokenType IDENTIFIER   = TokenType::IDENTIFIER;
constexpr TokenType INTEGER      = TokenType::INTEGER;
constexpr TokenType REAL         = TokenType::REAL;
constexpr TokenType STRING       = TokenType::STRING;
constexpr TokenType END_OF_FILE  = TokenType::END_OF_FILE;
constexpr TokenType ERROR        = TokenType::ERROR;

class Token
{
private:
    /**
     * The table (as a map) of reserved words. Initialize the table.
     */
    static map<string, TokenType> reservedWords;

public:
    /**
     * Initialize the static map.
     */
    static void initialize();

    TokenType type;  // what type of token
    int lineNumber;  // source line number of the token
    string text;     // text of the token
    Object value;    // the value (if any) of the token

    /**
     * Constructor.
     * @param firstChar the first character of the token.
     */
    Token(char firstChar) : type(ERROR), lineNumber(0), text("")
    {
        text += firstChar;
    }

    /**
     * Construct a word token.
     * @param firstChar the first character of the token.
     * @param source the input source.
     * @return the word token.
     */
    static Token *Word(char firstChar, Source *source);

    /**
     * Construct a number token and set its value.
     * @param firstChar the first character of the token.
     * @param source the input source.
     * @return the number token.
     */
    static Token *Number(char firstChar, Source *source);

    /**
     * Construct a string token and set its value.
     * @param firstChar the first character of the token.
     * @param source the input source.
     * @return the string token.
     */
    static Token *String(char firstChar, Source *source);

    /**
     * Construct a special symbol token and set its value.
     * @param firstChar the first character of the token.
     * @param source the input source.
     * @return the special symbol token.
     */
    static Token *SpecialSymbol(char firstChar, Source *source);

    static void tokenError(Token *token, string message);
};

}  // namespace frontend

#endif /* TOKEN_H_ */
