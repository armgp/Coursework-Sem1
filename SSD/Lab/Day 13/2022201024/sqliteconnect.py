import sqlite3
conn = sqlite3.connect("database.db")
conn.execute(''' 
    Create table user (
        id INT AUTO_INCREMENT PRIMARY KEY,
        name VARCHAR(20),
        email VARCHAR(20),
        password VARCHAR(80)
    )
''')
conn.close()
