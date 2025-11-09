#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "error.h"

static const KeywordEntry kw[] = {
    {"Char", Char}, {"False", False}, {"Float", Float}, {"Int", Int}, {"let", Let}, {"List", List}, {"match", Match}, {"True", True}, {"type", Type}, {"Unit", Unit}, {"with", With}, {"String", String}, {"effect", Effect}};

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

void initLex(Lex *lex, char *source)
{
    if (!source)
        exit(EXIT_FAILURE);
    lex->start = source;
    lex->current = source;
    lex->line = 1;
    lex->column = 1;
}

static Lex skipWhiteSpace(Lex lex)
{
    for (;;)
    {
        while (isspace(*lex.current))
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

static inline Token makeToken(TokenType type, char *start, Lex lex)
{
    return (Token){
        .typ = type,
        .start = start,
        .length = (int)(lex.current - start),
        .line = lex.line,
        .column = lex.column};
}

static Token parseIdentifier(Lex *lex)
{
    char *start = lex->current - 1;
    while (isalnum(*lex->current) || *lex->current == '_')
    {
        lex->current++;
        lex->column++;
    }
    TokenType type = checkKeyword(start, (size_t)(lex->current - start));
    return makeToken(type, start, *lex);
}

static Token parseNumber(Lex *lex)
{
    char *start = lex->current - 1;
    while (isdigit(*lex->current))
    {
        lex->current++;
        lex->column++;
    }

    TokenType type = IntLiteral;
    if (*lex->current == '.')
    {
        lex->current++;
        lex->column++;
        while (isdigit(*lex->current))
        {
            lex->current++;
            lex->column++;
        }
        type = FloatLiteral;
    }

    return makeToken(type, start, *lex);
}

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

Token getNextToken(Lex *lex)
{
    *lex = skipWhiteSpace(*lex);

    if (*lex->current == '\0')
        return makeToken(Eof, lex->current, *lex);

    char c = *lex->current++;
    lex->column++;

    if (isalpha(c) || c == '_')
        return parseIdentifier(lex);
    if (isdigit(c))
        return parseNumber(lex);
    if (c == '"')
        return parseString(lex);
    if (c == '\'')
        return parseChar(lex);

    return parseSymbol(lex, c);
}
