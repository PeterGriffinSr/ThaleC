/**
 * @file lex.c
 * @brief Implements the lexer for the Thale programming language.
 *
 * This file is responsible for tokenizing the source code into meaningful symbols
 * for further processing. It handles various language constructs and keywords,
 * ensuring accurate parsing and syntax analysis.
 *
 * @author Codezz-ops <codezz-ops@protonmail.com>
 *
 * @copyright Copyright (c) 2025 Codezz-ops
 */

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "error.h"

/**
 * @brief Array of keywords recognized by the lexer.
 *
 * Each entry maps a string keyword to its corresponding TokenType.
 */
static const KeywordEntry kw[] = {
    {"Char", Char}, {"False", False}, {"Float", Float}, {"Int", Int}, {"let", Let}, {"List", List}, {"match", Match}, {"True", True}, {"type", Type}, {"Unit", Unit}, {"with", With}, {"String", String}, {"effect", Effect}};

/**
 * @brief Checks if a given identifier matches a reserved keyword.
 *
 * Compares the input string with the known keyword list and returns
 * the corresponding TokenType if a match is found, otherwise returns Identifier.
 *
 * @param start Pointer to the beginning of the identifier string.
 * @param length Length of the identifier string.
 * @return TokenType The token type corresponding to the keyword or Identifier.
 */
static TokenType checkKeyword(const char *start, size_t length)
{
    for (size_t i = 0; i < sizeof(kw) / sizeof(kw[0]); i++)
    {
        const KeywordEntry *entry = &kw[i];
        if (strlen(entry->keyword) == length &&
            strncmp(entry->keyword, start, length) == 0)
            return entry->token;
    }
    return Identifier;
}

/**
 * @brief Initializes the lexer state.
 *
 * Sets the starting position, current position, line, and column of the lexer.
 *
 * @param lex Pointer to the lexer to initialize.
 * @param source The source code string to tokenize.
 */
void initLex(Lex *lex, char *source)
{
    if (!source)
        exit(EXIT_FAILURE);
    lex->start = source;
    lex->current = source;
    lex->line = 1;
    lex->column = 1;
}

/**
 * @brief Skips whitespace and comments in the source code.
 *
 * Advances the lexer's current pointer past spaces, tabs, newlines,
 * and single-line comments starting with "--".
 *
 * @param lex The lexer to advance.
 * @return Lex The updated lexer state after skipping whitespace.
 */
static Lex skipWhiteSpace(Lex lex)
{
    for (;;)
    {
        while (isspace((unsigned char)*lex.current))
        {
            if (*lex.current == '\n')
            {
                lex.line++;
                lex.column = 1;
            }
            else
            {
                lex.column++;
            }
            lex.current++;
        }

        if (lex.current[0] == '-' && lex.current[1] == '-')
        {
            lex.current += 2;
            lex.column += 2;
            while (*lex.current && *lex.current != '\n')
            {
                lex.current++;
                lex.column++;
            }
            continue;
        }

        break;
    }
    return lex;
}

/**
 * @brief Constructs a Token from the lexer state.
 *
 * @param type The type of the token.
 * @param start Pointer to the start of the token in the source code.
 * @param lex Current state of the lexer.
 * @return Token The newly created token.
 */
static inline Token makeToken(TokenType type, char *start, Lex lex)
{
    return (Token){
        .typ = type,
        .start = start,
        .length = (int)(lex.current - start),
        .line = lex.line,
        .column = lex.column};
}

/**
 * @brief Parses an identifier or keyword from the source code.
 *
 * Advances the lexer over alphanumeric characters and underscores,
 * then checks if the result is a keyword.
 *
 * @param lex Pointer to the lexer to advance.
 * @return Token The parsed identifier or keyword token.
 */
static Token parseIdentifier(Lex *lex)
{
    char *start = lex->current - 1;
    while (isalnum((unsigned char)*lex->current) || *lex->current == '_')
    {
        lex->current++;
        lex->column++;
    }
    TokenType type = checkKeyword(start, (size_t)(lex->current - start));
    return makeToken(type, start, *lex);
}

/**
 * @brief Parses an integer or floating-point number literal.
 *
 * @param lex Pointer to the lexer to advance.
 * @return Token The parsed numeric literal token (IntLiteral or FloatLiteral).
 */
static Token parseNumber(Lex *lex)
{
    char *start = lex->current - 1;
    while (isdigit((unsigned char)*lex->current))
    {
        lex->current++;
        lex->column++;
    }

    TokenType type = IntLiteral;
    if (*lex->current == '.')
    {
        lex->current++;
        lex->column++;
        while (isdigit((unsigned char)*lex->current))
        {
            lex->current++;
            lex->column++;
        }
        type = FloatLiteral;
    }

    return makeToken(type, start, *lex);
}

/**
 * @brief Parses a string literal.
 *
 * Handles escape sequences and reports lexical errors for invalid
 * escapes or unterminated strings.
 *
 * @param lex Pointer to the lexer to advance.
 * @return Token The parsed string literal token or an Unknown token on error.
 */
static Token parseString(Lex *lex)
{
    char *start = lex->current - 1;

    while (*lex->current && *lex->current != '"')
    {
        if (*lex->current == '\\')
        {
            lex->current++;
            lex->column++;

            switch (*lex->current)
            {
            case 'n':
            case 't':
            case 'r':
            case '\\':
            case '"':
                break;

            default:
                if (*lex->current)
                {
                    Token tok;
                    tok.typ = Unknown;
                    tok.start = lex->current - 1;
                    tok.length = 1;
                    tok.line = lex->line;
                    tok.column = lex->column - 1;

                    char msg[64];
                    snprintf(msg, sizeof(msg),
                             "Invalid escape sequence in string");
                    reportError(LexicalError, msg, lex, &tok);
                }
                break;
            }
        }

        if (*lex->current == '\n')
        {
            Token tok = makeToken(Unknown, start, *lex);
            reportError(LexicalError, "Unterminated string literal", lex, &tok);
            return tok;
        }

        if (*lex->current)
        {
            lex->current++;
            lex->column++;
        }
    }

    if (*lex->current == '"')
    {
        lex->current++;
        lex->column++;
        return makeToken(StringLiteral, start, *lex);
    }

    Token tok = makeToken(Unknown, start, *lex);
    reportError(LexicalError, "Unterminated string literal", lex, &tok);
    return tok;
}

/**
 * @brief Parses a character literal.
 *
 * Handles escape sequences and reports errors for invalid escapes
 * or unterminated character literals.
 *
 * @param lex Pointer to the lexer to advance.
 * @return Token The parsed char literal token or an Unknown token on error.
 */
static Token parseChar(Lex *lex)
{
    char *start = lex->current - 1;

    if (*lex->current == '\\')
    {
        lex->current++;
        lex->column++;

        switch (*lex->current)
        {
        case 'n':
        case 't':
        case 'r':
        case '\\':
        case '\'':
            break;

        default:
            if (*lex->current)
            {
                Token tok;
                tok.typ = Unknown;
                tok.start = lex->current - 1;
                tok.length = 1;
                tok.line = lex->line;
                tok.column = lex->column - 1;

                char msg[64];
                snprintf(msg, sizeof(msg),
                         "Invalid escape sequence in char literal");
                reportError(LexicalError, msg, lex, &tok);
            }
            break;
        }
    }

    if (*lex->current)
    {
        lex->current++;
        lex->column++;
    }

    if (*lex->current == '\'')
    {
        lex->current++;
        lex->column++;
        return makeToken(CharLiteral, start - 1, *lex);
    }

    Token tok = makeToken(Unknown, start, *lex);
    reportError(LexicalError, "Unterminated char literal", lex, &tok);
    return tok;
}

/**
 * @brief Parses a single-character or compound symbol token.
 *
 * Handles operators and punctuation such as +, -, &&, ||, ::, etc.
 * Reports an Unknown token error if the symbol is unrecognized.
 *
 * @param lex Pointer to the lexer to advance.
 * @param c The current character to parse as a symbol.
 * @return Token The parsed symbol token or an Unknown token on error.
 */
static Token parseSymbol(Lex *lex, char c)
{
    char *start = lex->current - 1;

    switch (c)
    {
    case '(':
        return makeToken(LParen, start, *lex);
    case ')':
        return makeToken(RParen, start, *lex);
    case '{':
        return makeToken(LBrace, start, *lex);
    case '}':
        return makeToken(RBrace, start, *lex);
    case '[':
        return makeToken(LBracket, start, *lex);
    case ']':
        return makeToken(RBracket, start, *lex);
    case '.':
        return makeToken(Dot, start, *lex);
    case ';':
        return makeToken(Semicolon, start, *lex);
    case ',':
        return makeToken(Comma, start, *lex);
    case '%':
        return makeToken(Percent, start, *lex);
    case '^':
        return makeToken(Carot, start, *lex);
    case '+':
        return makeToken(Plus, start, *lex);
    case '*':
        return makeToken(Star, start, *lex);
    case '/':
        return makeToken(Slash, start, *lex);
    case '=':
        return makeToken(Assign, start, *lex);
    case '>':
        return makeToken(Greater, start, *lex);
    case '<':
        if (*lex->current == '>')
        {
            lex->current++;
            lex->column++;
            return makeToken(NotEqual, start, *lex);
        }
        return makeToken(Less, start, *lex);
    case ':':
        if (*lex->current == ':')
        {
            lex->current++;
            lex->column++;
            return makeToken(ConsOP, start, *lex);
        }
        return makeToken(Colon, start, *lex);
    case '-':
        if (*lex->current == '>')
        {
            lex->current++;
            lex->column++;
            return makeToken(Arrow, start, *lex);
        }
        return makeToken(Minus, start, *lex);
    case '&':
        if (*lex->current == '&')
        {
            lex->current++;
            lex->column++;
            return makeToken(LogicalAnd, start, *lex);
        }
        return makeToken(Ampersand, start, *lex);
    case '|':
        if (*lex->current == '|')
        {
            lex->current++;
            lex->column++;
            return makeToken(LogicalOr, start, *lex);
        }
        return makeToken(Pipe, start, *lex);
    default:;
        Token tok = makeToken(Unknown, start, *lex);
        reportError(LexicalError, "Unknown symbol", lex, &tok);
        return tok;
    }
}

/**
 * @brief Retrieves the next token from the source code.
 *
 * Skips whitespace and comments, then determines the type of the next
 * token and delegates to the appropriate parsing function.
 *
 * @param lex Pointer to the lexer to advance.
 * @return Token The next token in the source code.
 */
Token getNextToken(Lex *lex)
{
    *lex = skipWhiteSpace(*lex);

    if (*lex->current == '\0')
        return makeToken(Eof, lex->current, *lex);

    char c = *lex->current++;
    lex->column++;

    if (isalpha((unsigned char)c) || c == '_')
        return parseIdentifier(lex);
    if (isdigit((unsigned char)c))
        return parseNumber(lex);
    if (c == '"')
        return parseString(lex);
    if (c == '\'')
        return parseChar(lex);

    return parseSymbol(lex, c);
}
