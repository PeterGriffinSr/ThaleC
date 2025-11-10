#ifndef HELP_H
#define HELP_H

/**
 * @file help.h
 * @brief Defines the command-line help functionality for the Thale compiler.
 *
 * This header file contains definitions for command-line commands,
 * their handlers, and functions to print the help menu and dispatch commands.
 *
 * Author: Codezz-ops <codezz-ops@protonmail.com>
 * Copyright (c) 2025 Codezz-ops
 */

/**
 * @typedef CommandHandler
 * @brief Function pointer type for command handlers.
 *
 * All command handlers must follow this signature.
 */
typedef int (*CommandHandler)(void);

/**
 * @struct Command
 * @brief Represents a command-line option for the compiler.
 *
 * Each command consists of a flag, description, and a handler function
 * that executes when the command is invoked.
 */

typedef struct
{
    const char *flag, *description;
    CommandHandler handler;
} Command;

/**
 * @brief Handles the `--help` or `-h` command.
 *
 * Prints the help menu to stdout.
 *
 * @return EXIT_SUCCESS.
 */
int handleHelp(void);

/**
 * @brief Handles the `--version` or `-v` command.
 *
 * Prints the compiler version.
 *
 * @return EXIT_SUCCESS.
 */
int handleVersion(void);

/**
 * @brief Prints the help menu to stdout.
 *
 * Iterates over all available commands and displays their flags
 * and descriptions.
 */
void printHelpMenu(void);

/**
 * @brief Dispatches a command based on argv input.
 *
 * Matches argv[1] against available command flags and calls the
 * corresponding handler.
 *
 * @param argc Argument count from main().
 * @param argv Argument vector from main().
 * @return Return code from the command handler, or -1 if no match.
 */
int dispatchCommand(int argc, char *argv[]);

#endif // HELP_H
