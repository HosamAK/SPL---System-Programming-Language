import sqlite3
import atexit
from dbtools import Dao

# Data Transfer Objects:
class Employee(object):
    #TODO: implement
    def __init__(self, conn):
        self.conn = conn
    
    def insert(self, employeeDTO):
        Dao.insert(employeeDTO)
        
    def find_all():
        Dao.find_all()
        
    def find(self, **keyvals):
        Dao.find(**keyvals)
    
    def delete(**keyvals):
        Dao.delete(**keyvals)
    
class Supplier(object):
    #TODO: implement
    def __init__(self, conn):
        self.conn = conn
    
    def insert(self, supplierDto):
        Dao.insert(supplierDto)
        
    def find_all():
        Dao.find_all()
        
    def find(self, **keyvals):
        Dao.find(**keyvals)
    
    def delete(**keyvals):
        Dao.delete(**keyvals)

class Product(object):
    #TODO: implement
    def __init__(self, conn):
        self.conn = conn
    
    def insert(self, productsDTO):
        Dao.insert(productsDTO)
        
    def find_all():
        Dao.find_all()
        
    def find(self, **keyvals):
        Dao.find(**keyvals)
    
    def delete(**keyvals):
        Dao.delete(**keyvals)

class Branche(object):
    #TODO: implement
    def __init__(self, conn):
        self.conn = conn
    
    def insert(self, brancheDTO):
        Dao.insert(brancheDTO)
        
    def find_all():
        Dao.find_all()
        
    def find(self, **keyvals):
        Dao.find(**keyvals)
    
    def delete(**keyvals):
        Dao.delete(**keyvals)

class Activitie(object):
    #TODO: implement
    def __init__(self, conn):
        self.conn = conn
    
    def insert(self, activitieDTO):
        Dao.insert(activitieDTO)
        
    def find_all():
        Dao.find_all()
        
    def find(self, **keyvals):
        Dao.find(**keyvals)
    
    def delete(**keyvals):
        Dao.delete(**keyvals)
 
 
#Repository
class Repository(object):
    def __init__(self):
        self._conn = sqlite3.connect('bgumart.db')
        self._conn.text_factory = bytes
        #TODO: complete
 
    def _close(self):
        self._conn.commit()
        self._conn.close()
 
    def create_tables(self):
        self._conn.executescript("""
            CREATE TABLE employees (
                id              INT         PRIMARY KEY,
                name            TEXT        NOT NULL,
                salary          REAL        NOT NULL,
                branche    INT REFERENCES branches(id)
            );
    
            CREATE TABLE suppliers (
                id                   INTEGER    PRIMARY KEY,
                name                 TEXT       NOT NULL,
                contact_information  TEXT
            );

            CREATE TABLE products (
                id          INTEGER PRIMARY KEY,
                description TEXT    NOT NULL,
                price       REAL NOT NULL,
                quantity    INTEGER NOT NULL
            );

            CREATE TABLE branches (
                id                  INTEGER     PRIMARY KEY,
                location            TEXT        NOT NULL,
                number_of_employees INTEGER
            );
    
            CREATE TABLE activities (
                product_id      INTEGER REFERENCES products(id),
                quantity        INTEGER NOT NULL,
                activator_id    INTEGER NOT NULL,
                date            TEXT    NOT NULL
            );
            
            CREATE TABLE employee_reports (
                id                   INT         PRIMARY KEY,
                name                 TEXT        NOT NULL,
                salary               REAL        NOT NULL,
                location             TEXT        NOT NULL,
                sales                REAL        NOT NULL
            );
        """)
        
    def execute_command(self, script: str) -> list:
        return self._conn.cursor().execute(script).fetchall()
 
# singleton
repo = Repository()
atexit.register(repo._close)