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
        assert(strncmp(token.start, input, (unsigned int)token.length) == 0);
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

void test_string(void)
{
    Lex lex;
    Token token;
    const char *inputs[] = {
        "\"hello\"", "\"world!\"", "\"\"", "\"123\"", "\"string with spaces\""};
    size_t numInputs = sizeof(inputs) / sizeof(inputs[0]);

    for (size_t i = 0; i < numInputs; ++i)
    {
        initLex(&lex, (char *)inputs[i]);
        token = getNextToken(&lex);
        assert(token.typ == StringLiteral);
        assert(token.length >= 2);
        token = getNextToken(&lex);
        assert(token.typ == Eof);
    }
}

void test_comment(void)
{
    Lex lex;
    Token token;
    const char *inputs[] = {
        "-- comment", "-- another comment with symbols!@#"};
    for (size_t i = 0; i < sizeof(inputs) / sizeof(inputs[0]); ++i)
    {
        initLex(&lex, (char *)inputs[i]);
        token = getNextToken(&lex);
        assert(token.typ == Eof);
    }
}

void test_mixed_sequence(void)
{
    Lex lex;
    Token token;
    const char *input = "let x = 42 + 3.14";
    initLex(&lex, (char *)input);

    TokenType expected[] = {
        Let, Identifier, Assign, IntLiteral, Plus, FloatLiteral, Eof};
    for (size_t i = 0;; ++i)
    {
        token = getNextToken(&lex);
        assert(token.typ == expected[i]);
        if (expected[i] == Eof)
            break;
    }
}

void test_char(void)
{
    Lex lex;
    Token token;
    const char *inputs[] = {"'a'", "'\\n'", "'\\t'", "'\\''", "'\\\\'"};
    for (size_t i = 0; i < sizeof(inputs) / sizeof(inputs[0]); ++i)
    {
        initLex(&lex, (char *)inputs[i]);
        token = getNextToken(&lex);
        assert(token.typ == CharLiteral);
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
    test_string();
    test_comment();
    test_mixed_sequence();
    test_char();
    return EXIT_SUCCESS;
}
