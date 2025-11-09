#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "lex.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

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
