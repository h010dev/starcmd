#!/bin/bash

sqlite3 ../data/starcmd-demo.db < ../data/starcmd-schema.sql;
sqlite3 ../data/starcmd-demo.db < ../data/starcmd-demo.sql;
