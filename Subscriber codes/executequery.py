from dbconnection import getconnection
 
def executeQuery(query):
    connection=getconnection()

    cursor=connection.cursor()

    cursor.execute(query)

    connection.commit()

    cursor.close()

    connection.close()