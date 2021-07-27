#!/bin/bash

sqlite3 starcmd-demo.db < starcmd-schema.sql;
sqlite3 starcmd-demo.db < starcmd-demo.sql;
