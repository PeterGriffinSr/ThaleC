#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error.h"

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
