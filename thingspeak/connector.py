import mysql.connector


def getconnection():
    connection = mysql.connector.connect(
        host = "127.0.0.1",
        port = 3306,
        user = "root",
        password = "root",
        database = "smart_environment_monitoring_system",
        use_pure = True
    )

    return connection