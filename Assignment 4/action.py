from persistence import *

import sys

def main(args : list):
    inputfilename : str = args[1]
    with open(inputfilename) as inputfile:
        for line in inputfile:
            splittedline : list[str] = line.strip().split(", ")
            repo._conn.execute("""
                        INSERT INTO activities (product_id, quantity, activator_id, date) VALUES (?, ?, ?, ?)
                            """, [splittedline[0], splittedline[1], splittedline[2], splittedline[3]])
            if splittedline[1] != "0":
                x = repo._conn.execute("""
                        SELECT quantity FROM products where id=?
                                         """, [splittedline[0]]).fetchone()
                
                if int(x[0])-int(splittedline[1])>=0:
                    quan = int(x[0]) - int(splittedline[1])
                    repo._conn.execute("""
                        UPDATE products SET quantity=? WHERE id=?
                                         """, [quan, splittedline[0]])

                
if __name__ == '__main__':
    main(sys.argv)