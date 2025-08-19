#! /usr/bin/env python3


"""
COMP3311
25T1
Assignment 2
Pokemon Database

Written by: Beonrik PASCUAL z5421297
Written on: 14/04/2025

File Name: Q2.py
"""


import sys
import psycopg2
import helpers


### Constants
USAGE = f"Usage: {sys.argv[0]}"

def main(db):
    ### Command-line args
    if len(sys.argv) != 1:
        print(USAGE)
        return 1

    cur = db.cursor()
    cur.execute('SELECT * FROM Type_Moves_Final_Count')
    type_data = cur.fetchall()
    type_name = 'TypeName'
    num_moves = '#Moves'
    num_mon = '#Pokemon'
    print(f"{type_name:<12} {num_moves:<8} {num_mon:<8}")
    for type_name, num_moves, num_mon in type_data:
        print(f"{type_name:<12} {num_moves:<8} {num_mon:<8}")


if __name__ == '__main__':
    exit_code = 0
    db = None
    try:
        db = psycopg2.connect(dbname="pkmon")
        exit_code = main(db)
    except psycopg2.Error as err:
        print("DB error: ", err)
        exit_code = 1
    except Exception as err:
        print("Internal Error: ", err)
        raise err
    finally:
        if db is not None:
            db.close()
    sys.exit(exit_code)
