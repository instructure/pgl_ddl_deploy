// From https://github.com/pganalyze/libpg_query/tree/156705b347d347c154fdfddf9341c07a9fa73dc2

// Copyright (c) 2015, Lukas Fittl <lukas@fittl.com>
// Copyright (c) 2016-2023, Duboce Labs, Inc. (pganalyze) <team@pganalyze.com>
// All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// * Redistributions of source code must retain the above copyright notice, this
// list of conditions and the following disclaimer.

// * Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following disclaimer in the documentation
// and/or other materials provided with the distribution.

// * Neither the name of pg_query nor the names of its contributors may be used
// to endorse or promote products derived from this software without specific
// prior written permission.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#ifndef POSTGRES_DEPARSE_H
#define POSTGRES_DEPARSE_H

#include <stdbool.h>
#include <sys/types.h>

typedef struct PostgresDeparseComment {
    int match_location;          // Insert comment before a node, once we find a node whose location field is equal-or-higher than this location
    int newlines_before_comment; // Insert newlines before inserting the comment (set to non-zero if the source comment was separated from the prior token by at least one newline)
    int newlines_after_comment;  // Insert newlines after inserting the comment (set to non-zero if the source comment was separated from the next token by at least one newline)
    char *str;                   // The actual comment string, including comment start/end tokens, and newline characters in comment (if any)
} PostgresDeparseComment;

typedef struct PostgresDeparseOpts {
    PostgresDeparseComment **comments;
    size_t comment_count;

    // Pretty print options
    bool pretty_print;
    int indent_size;           // Indentation size (Default 4 spaces)
    int max_line_length;       // Restricts the line length of certain lists of items (Default 80 characters)
    bool trailing_newline;     // Whether to add a trailing newline at the end of the output (Default off)
    bool commas_start_of_line; // Place separating commas at start of line (Default off)
} PostgresDeparseOpts;

/* Forward declarations to allow referencing the structs in this include file without needing Postgres includes */
struct StringInfoData;
typedef struct StringInfoData *StringInfo;
struct RawStmt;

extern void deparseRawStmt(StringInfo str, struct RawStmt *raw_stmt);
#if (PG_MAJORVERSION_NUM >= 18)
extern void deparseRawStmtOpts(StringInfo str, struct RawStmt *raw_stmt, PostgresDeparseOpts *opts);
#elif (PG_MAJORVERSION_NUM >= 17)
extern void deparseRawStmtOpts(StringInfo str, struct RawStmt *raw_stmt, PostgresDeparseOpts opts);
#endif

#endif