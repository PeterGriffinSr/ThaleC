/**
 * @file help.c
 * @brief Implements the command-line functionality for the Thale compiler.
 *
 * This header file contains definitions for command-line commands,
 * their handlers, and functions to print the help menu and dispatch commands.
 *
 * Author: Codezz-ops <codezz-ops@protonmail.com>
 * Copyright (c) 2025 Codezz-ops
 */

#include "help.h"
#include "config.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef THALE_VERSION
#ifdef VERSION
#define THALE_DISPLAY_VERSION VERSION
#else
#define THALE_DISPLAY_VERSION "unknown"
#endif
#else
#define THALE_DISPLAY_VERSION THALE_VERSION
#endif

/**
 * @brief Handles the `--help` or `-h` command.
 *
 * Calls printHelpMenu() to display all available options.
 *
 * @return EXIT_SUCCESS.
 */
int handleHelp(void)
{
    printHelpMenu();
    return EXIT_SUCCESS;
}

/**
 * @brief Handles the `--version` or `-v` command.
 *
 * Prints the Thale compiler version.
 *
 * @return EXIT_SUCCESS.
 */
int handleVersion(void)
{
    printf("Thale Compiler %s\n", THALE_DISPLAY_VERSION);
    return EXIT_SUCCESS;
}

/**
 * @brief List of all available commands.
 *
 * Each entry includes a flag, description, and the handler function.
 * The list is NULL-terminated.
 */
static const Command commands[] = {
    {"-h", "Display this help message", handleHelp},
    {"--help", "Display this help message", handleHelp},
    {"-v", "Show compiler version", handleVersion},
    {"--version", "Show compiler version", handleVersion},
    {NULL, NULL, NULL}};

/**
 * @brief Prints the help menu to stdout.
 *
 * Iterates over the `commands` array and prints each flag and description.
 */
void printHelpMenu(void)
{
    printf("Usage: thale [options] <input_file>\n\n");
    printf("Options:\n");
    for (int i = 0; commands[i].flag != NULL; i++)
    {
        printf("  %-15s %s\n", commands[i].flag, commands[i].description);
    }
}

/**
 * @brief Dispatches a command based on the first argument.
 *
 * Checks argv[1] against available commands and executes the matching handler.
 *
 * @param argc Argument count from main().
 * @param argv Argument vector from main().
 * @return Return code from the command handler, or -1 if no match.
 */
int dispatchCommand(int argc, char *argv[])
{
    if (argc < 2)
        return handleHelp();

    for (int i = 0; commands[i].flag != NULL; i++)
    {
        if (strcmp(argv[1], commands[i].flag) == 0)
        {
            return commands[i].handler();
        }
    }

    return -1;
}