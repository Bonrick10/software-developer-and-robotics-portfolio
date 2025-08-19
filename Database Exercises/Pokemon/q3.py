#! /usr/bin/env python3


"""
COMP3311
25T1
Assignment 2
Pokemon Database

Written by: <YOUR NAME HERE> <YOUR STUDENT ID HERE>
Written on: <DATE HERE>

File Name: Q3
"""


import sys
import psycopg2
import helpers


### Constants
USAGE = f"Usage: {sys.argv[0]} <pokemon_name>"


def main(db):
    ### Command-line args
    if len(sys.argv) != 2:
        print(USAGE)
        return 1

    pokemon_name = sys.argv[1]

    cur = db.cursor()

    select_func = "SELECT * FROM Check_Mon_Exists(%s)"
    cur.execute(select_func, [pokemon_name])
    check_mon = cur.fetchall()
    if len(check_mon) == 0:
        print(f'Pokemon "{pokemon_name}" not found')
        return 0

    select_func = "SELECT * FROM Get_Mon_Mov_Req_Data(%s)"
    cur.execute(select_func, [pokemon_name])
    mon_data = cur.fetchall()
    if len(mon_data) == 0:
        print(f'No moves learnable in at least 30 games found for Pokemon "{pokemon_name}"')
        return 0

    move_name = 'MoveName'
    num_games = '#Games'
    avg_lvl = '#AvgLearntLevel'

    print(f"{move_name:<16} {num_games:<6} {avg_lvl:<16}")

    for move_name, num_games, avg_lvl in mon_data:
        print(f"{move_name:<16} {num_games:<6} {avg_lvl:<16}")



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
