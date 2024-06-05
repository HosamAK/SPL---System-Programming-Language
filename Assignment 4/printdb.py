from persistence import *

def main():
    print("Activities")
    activ = repo._conn.execute("""
        SELECT * FROM activities
                       """).fetchall()
    for toPrint in activ:
        print("({}, {}, {}, {})".format(toPrint[0], toPrint[1], toPrint[2], toPrint[3]))
    
    pass

if __name__ == '__main__':
    main()