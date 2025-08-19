#! /usr/bin/env python3


"""
COMP3311
25T1
Assignment 2
Pokemon Database

Written by: Beonrik PASCUAL z5421297
Written on: 20/04/2026

File Name: Q4
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

    select_func = "SELECT * FROM Check_Partial_Mon_Exists(%s)"
    cur.execute(select_func, [pokemon_name])
    check_mon = cur.fetchall()
    if len(check_mon) == 0:
        print(f'Pokemon "{pokemon_name}" not found')
        return 0

    select_func = "SELECT * FROM Get_Mon_Evo_Data(%s)"
    cur.execute(select_func, [pokemon_name])
    mon_data = cur.fetchall()

    prev_mon_name = None
    prev_depth = None
    is_first = True
    for mon_name, evo_name, evo_depth, evo_id, evo_req, evo_inv in mon_data:
        
        if mon_name != prev_mon_name:
            prev_evo_id = None
            
            if prev_depth == 0:
                print('\n- No Evolutions',end='')
            
            if is_first:
                print(f'{mon_name}: The full evolution chain:', end='')
            else:
                print(f'\n{mon_name}: The full evolution chain:', end='')

            is_first = False
            prev_mon_name = mon_name

        if evo_depth == 0:
            print(f'\n{evo_name}', end='')
        else:
            if evo_id == prev_evo_id:
                print(' AND ', end='')
            else:
                print('\n', end='')
                print('+' * evo_depth, end=' ')
                print(f'For "{evo_name}", The evolution requirement is ', end='')
                prev_evo_id = evo_id

            if evo_inv:
                print(f'[Not {evo_req}]', end='')
            else:
                print(f'[{evo_req}]', end='')
            
        prev_depth = evo_depth
    
    if prev_depth == 0:
        print('\n- No Evolutions',end='')
    print('\n', end='')
        

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
