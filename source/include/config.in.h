/**
 * @file config.h.in
 * @brief Template configuration header for the Thale compiler.
 *
 * This file is used by the build system to generate `config.h` with
 * the correct version number. The `@PACKAGE_VERSION@` placeholder is
 * replaced with the actual package version during configuration.
 *
 * Author: Codezz-ops <codezz-ops@protonmail.com>
 * Copyright (c) 2025 Codezz-ops
 */

/**
 * @brief Compiler version string.
 *
 * This macro is replaced by the actual version number during the
 * configuration process.
 */

#define THALE_VERSION "@PACKAGE_VERSION@"
