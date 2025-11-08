#ifndef LEX_H
#define LEX_H

#include <stddef.h>

typedef enum
{
    LParen = 1,
    RParen,
    LBrace,
    RBrace,
    LBracket,
    RBracket,
    Dot,
    Colon,
    Semicolon,
    Comma,
    Percent,
    Carot,
    Plus,
    Minus,
    Star,
    Slash,
    NotEqual,
    Assign,
    Ampersand,
    LogicalAnd,
    Pipe,
    LogicalOr,
    Greater,
    Less,
    ConsOP,
    Arrow,
    Identifier,
    IntLiteral,
    FloatLiteral,
    StringLiteral,
    CharLiteral,
    Char,
    False,
    Float,
    Int,
    Let,
    List,
    Match,
    True,
    Type,
    Unit,
    With,
    Unknown,
    Eof
} TokenType;

typedef struct
{
    TokenType typ;
    char *start;
    int length, line, column;
} Token;

typedef struct
{
    char *start, *current;
    int line, column;
} Lex;

typedef struct
{
    const char *keyword;
    TokenType token;
} KeywordEntry;

void initLex(Lex *lex, char *source);
Token getNextToken(Lex *lex);

#endif
