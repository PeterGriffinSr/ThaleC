#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "include/lex_tests.h"
#include "../src/include/lex.h"

void test_identifier(void)
{
    Lex lex;
    Token token;
    const char *identifiers[] = {"variable", "myVar", "test123", "another_var", "x", "y", "z", "longIdentifierName", "short", "temp", "_start", "snake_case_var", "my_long_variable_name", "_", "___", "number_123", "UPPER_SNAKE_CASE", "mixed_Snake_Case_123"};
    size_t numIdentifiers = sizeof(identifiers) / sizeof(identifiers[0]);

    for (size_t i = 0; i < numIdentifiers; ++i)
    {
        const char *input = identifiers[i];
        initLex(&lex, (char *)input);
        token = getNextToken(&lex);

        assert(token.typ == Identifier);
        assert(strncmp(token.start, input, strlen(input)) == 0);

        token = getNextToken(&lex);
        assert(token.typ == Eof);
    }
}

void test_number(void)
{
    Lex lex;
    Token token;
    const char *numbers[] = {"123", "12.3", "0", "0.0", "456", "78.90", "0.123", "123.0"};
    TokenType expectedTokens[] = {IntLiteral, FloatLiteral, IntLiteral, FloatLiteral, IntLiteral, FloatLiteral, FloatLiteral, FloatLiteral};
    size_t numInputs = sizeof(numbers) / sizeof(numbers[0]);

    for (size_t i = 0; i < numInputs; ++i)
    {
        const char *input = numbers[i];
        initLex(&lex, (char *)input);
        token = getNextToken(&lex);

        assert(token.typ == expectedTokens[i]);
        assert(strcmp(token.start, input) == 0);

        token = getNextToken(&lex);
        assert(token.typ == Eof);
    }
}

void test_operator(void)
{
    Lex lex;
    Token token;
    const char *inputs[] = {"(", ")", "{", "}", "[", "]", ".", ":", ";", ",", "%", "+", "-", "*", "/", "<>", "=", "&", "&&", "|", "||", ">", "<", "::", "->"};
    TokenType expectedTokens[] = {LParen, RParen, LBrace, RBrace, LBracket, RBracket, Dot, Colon, Semicolon, Comma, Percent, Plus, Minus, Star, Slash, NotEqual, Assign, Ampersand, LogicalAnd, Pipe, LogicalOr, Greater, Less, ConsOP, Arrow};
    size_t numOperators = sizeof(inputs) / sizeof(inputs[0]);

    for (size_t i = 0; i < numOperators; ++i)
    {
        const char *input = inputs[i];
        initLex(&lex, (char *)input);
        token = getNextToken(&lex);

        assert(token.typ == expectedTokens[i]);
        assert(strncmp(token.start, input, token.length) == 0);
    }
}

void test_keyword(void)
{
    Lex lex;
    Token token;
    const char *keywords[] = {"Char", "False", "Float", "Int", "let", "List", "match", "True", "type", "Unit", "with", "String"};
    TokenType expectedTokens[] = {Char,
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
                                  String};
    size_t num = sizeof(keywords) / sizeof(keywords[0]);

    for (size_t i = 0; i < num; ++i)
    {
        initLex(&lex, (char *)keywords[i]);
        token = getNextToken(&lex);
        assert(token.typ == expectedTokens[i]);
        token = getNextToken(&lex);
        assert(token.typ == Eof);
    }
}

int main(void)
{
    test_identifier();
    test_operator();
    test_number();
    test_keyword();
    return EXIT_SUCCESS;
}