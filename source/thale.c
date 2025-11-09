/**
 * @file thale.c
 * @brief Main entry point for the Thale compiler.
 *
 * This file contains the main function and initialization logic for the
 * Thale compiler. It handles command-line arguments, sets up the
 * environment, and starts the compilation process.
 *
 * @author Codezz-ops <codezz-ops@protonmail.com>
 *
 * @copyright Copyright (c) 2025 Codezz-ops
 */

#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "lex.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

/**
 * @brief The main entry point of the Thale compiler.
 *
 * This function processes command-line arguments, opens the input file,
 * reads its contents into memory, initializes the lexer, and retrieves
 * tokens from the source code until the end of the file is reached.
 *
 * @param argc The number of command-line arguments.
 * @param argv An array of command-line argument strings.
 * @return int Returns EXIT_SUCCESS on successful execution, or EXIT_FAILURE on error.
 */
int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fputs("thale: error: no input file.\n", stderr);
        return EXIT_FAILURE;
    }

    long length;
    FILE *file;
    char *buffer;
    Lex lex;
    size_t bytesRead;

    file = fopen(argv[1], "r");
    if (file == NULL)
    {
        fputs("thale: error: could not read file.\n", stderr);
        return EXIT_FAILURE;
    }

    fseek(file, 0, SEEK_END);
    length = ftell(file);
    rewind(file);

    buffer = (char *)malloc((size_t)(length + 1));
    if (buffer == NULL)
    {
        fprintf(stderr, "thale: error: could not allocate memory for file '%s'\n", argv[1]);
        return EXIT_FAILURE;
    }

    bytesRead = fread(buffer, 1, (size_t)(length), file);
    if (bytesRead != (size_t)length)
    {
        fprintf(stderr, "thale: error: could not read file '%s'\n", argv[1]);
        free(buffer);
        fclose(file);
        return EXIT_FAILURE;
    }
    buffer[length] = '\0';

    initLex(&lex, buffer);

    while (true)
    {
        Token token = getNextToken(&lex);
        if (token.typ == Eof)
        {
            break;
        }

        printf("Token: %d\n", token.typ);
    }

    fclose(file);
    free(buffer);

    return EXIT_SUCCESS;
}
