/**
 * @file error.c
 * @brief Implements the error handling system for the Thale programming language.
 *
 * This file is responsible for managing error reporting and handling within the
 * Thale compiler. It defines error types, messages, and functions to report
 * errors encountered during compilation and execution.
 *
 * @author Codezz-ops <codezz-ops@protonmail.com>
 *
 * @copyright Copyright (c) 2025 Codezz-ops
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error.h"

/**
 * @brief Converts an ErrorType enum to a string representation.
 *
 * @param type The ErrorType to convert.
 * @return const char* A string representation of the error type.
 */
const char *errorTypeToString(ErrorType type)
{
    switch (type)
    {
    case LexicalError:
        return "LexicalError";
    case SyntaxError:
        return "SyntaxError";
    case SemanticError:
        return "SemanticError";
    default:
        return "UnknownError";
    }
}

/**
 * @brief Prints an error message to stderr with context information.
 *
 * This function formats and displays an error message, including the type of error,
 * the line and column where the error occurred, and a snippet of the source code
 * around the error. It also handles color formatting for terminal output based on
 * the operating system.
 *
 * @param type The type of error that occurred.
 * @param message A message describing the error.
 * @param lex A pointer to the Lexer structure.
 * @param token A pointer to the Token structure that contains information about
 *              the location of the error in the source code.
 * @return void.
 */
void reportError(ErrorType type, const char *message, const Lex *lex, const Token *token)
{
    const char *src = lex->start;
    const char *line_start = token->start;
    while (line_start > src && line_start[-1] != '\n')
        line_start--;

    const char *line_end = token->start;
    while (*line_end != '\n' && *line_end != '\0')
        line_end++;

    int column = (int)(token->start - line_start) + 1;

    fprintf(stderr, "%s: [line %d, column %d] %s",
            errorTypeToString(type),
            token->line,
            column,
            message);

    fputc('\n', stderr);

    fprintf(stderr, "    %d | %.*s\n", token->line, (int)(line_end - line_start), line_start);

    fputs("      | ", stderr);
    for (int i = 1; i < column; i++)
        fputc(' ', stderr);
    fputs("^\n", stderr);

    exit(EXIT_FAILURE);
}
