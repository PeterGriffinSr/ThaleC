#ifndef LEX_H
#define LEX_H

/**
 * @file lex.h
 * @brief Defines the lexer for the Thale programming language.
 *
 * This header file contains the definitions for the lexer, including token types,
 * structures for tokens and the lexer itself, and function declarations for
 * initializing the lexer and retrieving tokens from the source code.
 *
 * @author Codezz-ops <codezz-ops@protonmail.com>
 *
 * @copyright Copyright (c) 2025 Codezz-ops
 */

#include <stddef.h>

/**
 * @enum TokenType
 * @brief Enumeration of token types recognized by the lexer.
 *
 * This enumeration defines the various token types that the lexer can recognize,
 * including operators, keywords, literals, and special tokens.
 */
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
    String,
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
    Effect,
    Unknown,
    Eof
} TokenType;

/**
 * @struct Token
 * @brief Represents a token recognized by the lexer.
 *
 * This structure holds information about a token, including its type,
 * the starting position in the source code, its length, and its line
 * and column numbers.
 */
typedef struct
{
    TokenType typ;
    char *start;
    int length, line, column;
} Token;

/**
 * @struct Lex
 * @brief Represents the lexer state.
 *
 * This structure holds the current state of the lexer, including the
 * current position in the source code and the line and column numbers.
 */
typedef struct
{
    char *start, *current;
    int line, column;
} Lex;

/**
 * @struct KeywordEntry
 * @brief Represents a keyword entry for the lexer.
 *
 * This structure holds a keyword and its corresponding token type.
 */
typedef struct
{
    const char *keyword;
    TokenType token;
} KeywordEntry;

/**
 * @brief Initializes the lexer with the source code.
 *
 * This function sets the starting position, current position, line number,
 * and column number for the lexer based on the provided source code.
 *
 * @param lex Pointer to the lexer instance to initialize.
 * @param source Pointer to the source code string.
 */
void initLex(Lex *lex, char *source);

/**
 * @brief Retrieves the next token from the lexer.
 *
 * This function analyzes the source code and returns the next token
 * recognized by the lexer.
 *
 * @param lex Pointer to the lexer instance.
 * @return Token The next token recognized by the lexer.
 */
Token getNextToken(Lex *lex);

#endif // LEX_H
