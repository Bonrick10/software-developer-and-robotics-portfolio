#! /usr/bin/env python3


"""
COMP3311
25T1
Assignment 2
Pokemon Database

Written by: Beonrik Pascual z5421297
Written on: 09/04/2025

File Name: Q1.py
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
    cur.execute('SELECT * FROM mon_egg_count')
    game_data = cur.fetchall()
    game_name = 'GameName'
    num_egg = '#EggGroup'
    num_mon = '#Pokemon'
    print(f"{game_name:<17} {num_egg:<9} {num_mon:<8}")
    for game_name, num_egg, num_mon in game_data:
        print(f"{game_name:<17} {num_egg:<9} {num_mon:<8}")


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
