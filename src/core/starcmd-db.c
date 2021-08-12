/*
 * Starcmd Command Browser
 *
 * Copyright © 2021 Mohamed Al-Hussein <mohamed.n.al.hussein@gmail.com>
 * 
 * SPDX-License-Identifier: GPL-3.0-only
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "stdlib.h"
#include "stdio.h"
#include "string.h"

#include "starcmd-db.h"

static const char *f_db = "../data/starcmd-demo.db";

int
starcmd_db_open (sqlite3 **db)
{
    if (sqlite3_open (f_db, db) != SQLITE_OK)
    {
        fprintf (stderr, "Cannot open database: %s\n", sqlite3_errmsg (*db));
        sqlite3_close (*db);
        return 1;
    }
    return 0;
}

int
starcmd_db_load (sqlite3 **db, char *name, struct CommandData **data)
{
    int           rc;
    sqlite3_stmt *res;
    char         *sql;

    sql = "SELECT name, platform, os, description, command, examples, refs, tags, datemod "
          "FROM commands "
          "WHERE name = ?";

    if ( (rc = sqlite3_prepare_v2 (*db, sql, -1, &res, 0)) != SQLITE_OK)
    {
        fprintf (stderr, "Cannot execute statement: %s\n", sqlite3_errmsg (*db));
        return 1;
    }

    sqlite3_bind_text (res, 1, name, strlen (name), SQLITE_TRANSIENT);

    int step;
    if ( (step = sqlite3_step (res)) == SQLITE_ROW)
    {
        (*data)->name = (char *) strdup ((const char *) sqlite3_column_text (res, 0));
        (*data)->platform = (char *) strdup ((const char *) sqlite3_column_text (res, 1));
        (*data)->os = (char *) strdup ((const char *) sqlite3_column_text (res, 2));
        (*data)->description = (char *) strdup ((const char *) sqlite3_column_text (res, 3));
        (*data)->commands = (char *) strdup ((const char *) sqlite3_column_text (res, 4));
        (*data)->examples = (char *) strdup ((const char *) sqlite3_column_text (res, 5));
        (*data)->references = (char *) strdup ((const char *) sqlite3_column_text (res, 6));
        (*data)->tags = (char *) strdup ((const char *) sqlite3_column_text (res, 7));
        (*data)->datemod = (char *) strdup ((const char *) sqlite3_column_text (res, 8));
    }
    sqlite3_finalize (res);

    return 0;
}

int
starcmd_db_loadall (sqlite3 **db, struct CommandData data[], int count)
{
    int           rc;
    sqlite3_stmt *res;
    char         *sql;

    sql = "SELECT name, platform, os, description, command, examples, refs, tags, datemod, icon "
          "FROM commands "
          "ORDER BY name ASC;";

    if ( (rc = sqlite3_prepare_v2 (*db, sql, -1, &res, 0)) != SQLITE_OK)
    {
        fprintf (stderr, "Cannot execute statement: %s\n", sqlite3_errmsg (*db));
        return 1;
    }

    int i = 0;
    while (sqlite3_step (res) == SQLITE_ROW && i < count)
    {
        data[i].name = (char *) strdup ((const char *) sqlite3_column_text (res, 0));
        data[i].platform = (char *) strdup ((const char *) sqlite3_column_text (res, 1));
        data[i].os = (char *) strdup ((const char *) sqlite3_column_text (res, 2));
        data[i].description = (char *) strdup ((const char *) sqlite3_column_text (res, 3));
        data[i].commands = (char *) strdup ((const char *) sqlite3_column_text (res, 4));
        data[i].examples = (char *) strdup ((const char *) sqlite3_column_text (res, 5));
        data[i].references = (char *) strdup ((const char *) sqlite3_column_text (res, 6));
        data[i].tags = (char *) strdup ((const char *) sqlite3_column_text (res, 7));
        data[i].datemod = (char *) strdup ((const char *) sqlite3_column_text (res, 8));
        data[i].icon_path = (char *) strdup ((const char *) sqlite3_column_text (res, 9));

        i++;
    }
    sqlite3_finalize (res);

    return 0;
}

int
starcmd_db_save (sqlite3 **db, char *name, struct CommandData *data)
{
    int           rc;
    sqlite3_stmt *res;
    char         *sql;

    if (name == NULL)
        sql = "INSERT INTO COMMANDS (name, platform, os, description, command, examples, refs, tags, datemod, icon)"
              "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";
    else
        sql = "UPDATE commands " 
              "SET name = ?, platform = ?, os = ?, description = ?, command = ?, examples = ?, refs = ?, tags = ?, datemod = ?, icon = ? "
              "WHERE name = ?";

    if ( (rc = sqlite3_prepare_v2 (*db, sql, -1, &res, 0)) != SQLITE_OK)
    {
        fprintf (stderr, "Cannot execute statement: %s\n", sqlite3_errmsg (*db));
        return 1;
    }

    sqlite3_bind_text (res, 1, data->name, strlen (data->name), SQLITE_TRANSIENT);
    sqlite3_bind_text (res, 2, data->platform, strlen (data->platform), SQLITE_TRANSIENT);
    sqlite3_bind_text (res, 3, data->os, strlen (data->os), SQLITE_TRANSIENT);
    sqlite3_bind_text (res, 4, data->description, strlen (data->description), SQLITE_TRANSIENT);
    sqlite3_bind_text (res, 5, data->commands, strlen (data->commands), SQLITE_TRANSIENT);
    sqlite3_bind_text (res, 6, data->examples, strlen (data->examples), SQLITE_TRANSIENT);
    sqlite3_bind_text (res, 7, data->references, strlen (data->references), SQLITE_TRANSIENT);
    sqlite3_bind_text (res, 8, data->tags, strlen (data->tags), SQLITE_TRANSIENT);
    sqlite3_bind_text (res, 9, data->datemod, strlen (data->datemod), SQLITE_TRANSIENT);
    sqlite3_bind_text (res, 10, data->icon_path, strlen (data->icon_path), SQLITE_TRANSIENT);

    if (name != NULL)
        sqlite3_bind_text (res, 11, name, strlen (name), SQLITE_TRANSIENT);

    sqlite3_step (res);
    sqlite3_finalize (res);

    return 0;
}

int
starcmd_db_delete (sqlite3 **db, char *name)
{
    int           rc;
    sqlite3_stmt *res;
    char         *sql;

    sql = "DELETE FROM commands "
          "WHERE name = ?";

    if ( (rc = sqlite3_prepare_v2 (*db, sql, -1, &res, 0)) != SQLITE_OK)
    {
        fprintf (stderr, "Cannot execute statement: %s\n", sqlite3_errmsg (*db));
        return 1;
    }

    sqlite3_bind_text (res, 1, name, strlen (name), SQLITE_TRANSIENT);
    sqlite3_step (res);
    sqlite3_finalize (res);

    return 0;
}

int
starcmd_db_count (sqlite3 **db, int *count)
{
    int           rc;
    sqlite3_stmt *res;
    char         *sql;

    sql = "SELECT COUNT (*) FROM commands;";

    if ( (rc = sqlite3_prepare_v2 (*db, sql, -1, &res, 0)) != SQLITE_OK)
    {
        fprintf (stderr, "Cannot execute statement: %s\n", sqlite3_errmsg (*db));
        return 1;
    }

    int step;
    if ( (step = sqlite3_step (res)) == SQLITE_ROW)
        *count = sqlite3_column_int (res, 0);
    sqlite3_finalize (res);

    return 0;
}

int
starcmd_db_close (sqlite3 **db)
{
    sqlite3_close (*db);
    return 0;
}

void
free_command_data (struct CommandData **data)
{
    free ((*data)->name);
    free ((*data)->platform);
    free ((*data)->os);
    free ((*data)->description);
    free ((*data)->commands);
    free ((*data)->examples);
    free ((*data)->references);
    free ((*data)->tags);
    free ((*data)->datemod);
    free (*data);
}

