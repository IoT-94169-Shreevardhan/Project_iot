from dbconnection import getconnection

def executeSelectQuery(query):
     connection=getconnection()

     cursor=connection.cursor()

     cursor.execute(query)

     data=cursor.fetchone()

     cursor.close()

     connection.close()

     return data