#!/bin/bash

mkdir -p ~/.config/StarCMD;
mkdir -p ~/.config/StarCMD/data;
sqlite3 ~/.config/StarCMD/data/userdata.db < data/starcmd-schema.sql;
