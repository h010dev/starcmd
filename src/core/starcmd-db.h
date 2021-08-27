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

#ifndef STARCMD_DB_H
#define STARCMD_DB_H

#include "sqlite3.h"

struct CommandData
{
    char *name;
    char *platform;
    char *os;
    char *description;
    char *commands;
    char *examples;
    char *references;
    char *tags;
    char *datemod;
};

int  starcmd_db_open    (sqlite3 **db);
int  starcmd_db_load    (sqlite3 **db, char *name, struct CommandData **data);
int  starcmd_db_loadall (sqlite3 **db, struct CommandData data[], int count);
int  starcmd_db_save    (sqlite3 **db, char *name, struct CommandData *data);
int  starcmd_db_delete  (sqlite3 **db, char *name);
int  starcmd_db_close   (sqlite3 **db);
int  starcmd_db_count   (sqlite3 **db, int *count);
void free_command_data  (struct CommandData **data);

#endif /* STARCMD_DB_H */
