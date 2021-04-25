#!/bin/env python3

import recipes_to_sql
from subprocess import Popen, PIPE, STDOUT
import sys
import os

if __name__ == '__main__':
    if len(sys.argv) != 2:
        print("usage: init_db /path/to/recipes.db")
        exit(1)
    if os.path.isfile(sys.argv[1]):
        os.unlink(sys.argv[1])
    sql = open('db.sql').read()
    sql += recipes_to_sql.ToSql(recipes_to_sql.ParseRecipes())
    p = Popen(["sqlite3", sys.argv[1]], stdout=PIPE, stdin=PIPE, stderr=STDOUT)   
    out = p.communicate(input=bytes(sql, "utf8"))
    print(out)
