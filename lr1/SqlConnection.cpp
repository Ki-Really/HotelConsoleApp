#include "SQLConnection.h"
 


SQLConnection::SQLConnection() {

}

SQLConnection::~SQLConnection() {
    SQLDisconnect(this->connection);
    SQLFreeHandle(SQL_HANDLE_ENV, this->env);
    SQLFreeHandle(SQL_HANDLE_DBC, this->connection);
    
}

SQLRETURN SQLConnection::startTransaction() {
    return SQLSetConnectAttr(
        this->connection,
        SQL_ATTR_AUTOCOMMIT,
        (SQLPOINTER)SQL_AUTOCOMMIT_OFF,
        SQL_IS_INTEGER
    );
}

SQLRETURN SQLConnection::commitTransaction() {
    SQLEndTran(SQL_HANDLE_DBC, this->connection, SQL_COMMIT);
    return SQLSetConnectAttr(
        this->connection,
        SQL_ATTR_AUTOCOMMIT,
        (SQLPOINTER)SQL_AUTOCOMMIT_ON,
        SQL_IS_INTEGER
    );
}

SQLRETURN SQLConnection::rollbackTransaction() {
    SQLEndTran(SQL_HANDLE_DBC, this->connection, SQL_ROLLBACK);
    return SQLSetConnectAttr(
        this->connection,
        SQL_ATTR_AUTOCOMMIT,
        (SQLPOINTER)SQL_AUTOCOMMIT_ON,
        SQL_IS_INTEGER
    );
}


SQLRETURN SQLConnection::init(SQLCHAR* dsn, SQLCHAR* user, SQLCHAR* password) {

    SQLRETURN returnCode = SQLAllocHandle(
        SQL_HANDLE_ENV,
        SQL_NULL_HANDLE,
        &(this->env)
    );
    if (returnCode != SQL_SUCCESS) return returnCode;

    std::wcout << "Env handle allocated" << std::endl;

    returnCode = SQLSetEnvAttr(
        this->env,
        SQL_ATTR_ODBC_VERSION,
        (void*)SQL_OV_ODBC3, SQL_NTS
    );
    if (returnCode != SQL_SUCCESS) return returnCode;

    std::wcout << "Env attr set" << std::endl;

    returnCode = SQLAllocHandle(
        SQL_HANDLE_DBC,
        env,
        &(this->connection)
    );
    if (returnCode != SQL_SUCCESS) return returnCode;

    std::wcout << "Connection allocated" << std::endl;

    returnCode = SQLConnect(
        connection,
        (SQLWCHAR*)dsn, SQL_NTS,
        (SQLWCHAR*)user, SQL_NTS,
        (SQLWCHAR*)password, SQL_NTS
    );
    if (returnCode != SQL_SUCCESS)  { 
        return returnCode;
    }

    std::wcout << "Connected" << std::endl;

    SQLWCHAR dbms_name[256], dbms_ver[256];
    SQLUINTEGER supports;
    SQLUSMALLINT max_concur_act;
    SQLSMALLINT string_len;

    //Сбор данных из бд. Что она поддерживает и как с ней работать
    SQLGetInfo(
        connection,
        SQL_DBMS_NAME, (SQLPOINTER)dbms_name,
        sizeof(dbms_name), NULL
    );
    SQLGetInfo(
        connection,
        SQL_DBMS_VER, (SQLPOINTER)dbms_ver,
        sizeof(dbms_ver), NULL
    );
    SQLGetInfo(
        connection,
        SQL_GETDATA_EXTENSIONS, (SQLPOINTER)&supports,
        0, 0
    );
    SQLGetInfo(
        connection,
        SQL_MAX_CONCURRENT_ACTIVITIES, &max_concur_act,
        0, 0
    );

    std::wcout << "DBMS Name:    " << dbms_name << std::endl;
    std::wcout << "DBMS Version: " << dbms_ver << std::endl;
   
    if (max_concur_act == 0) {
        std::wcout << "SQL_MAX_CONCURRENT_ACTIVITIES - no limit or undefined" << std::endl;
    }
    else {
        std::wcout << "SQL_MAX_CONCURRENT_ACTIVITIES = " << max_concur_act << std::endl;
    }

    if (supports & SQL_GD_ANY_ORDER) {
        std::wcout << "SQLGetData - columns can be retrieved in any order" << std::endl;
    }
    else {
        std::wcout << "SQLGetData - columns must be retrieved in order" << std::endl;
    }

    if (supports & SQL_GD_ANY_COLUMN) {
        std::wcout << "SQLGetData - can retrieve columns before last bound one" << std::endl;
    }
    else {
        std::wcout << "SQLGetData - columns must be retrieved after last bound one" << std::endl;
    }


    return SQL_SUCCESS;
}





