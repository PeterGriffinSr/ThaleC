#ifndef ERROR_H
#define ERROR_H

#include "lex.h"

typedef enum
{
    LexicalError,
    SyntaxError,
    SemanticError
} ErrorType;

const char *errorTypeToString(ErrorType type);

void reportError(ErrorType type, const char *message, const Lex *lex, const Token *token);

#endif
