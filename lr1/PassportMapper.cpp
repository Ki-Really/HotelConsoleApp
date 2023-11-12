#include "PassportMapper.h"
#include "ShowError.h"

PassportMapper::PassportMapper(SQLConnection* connection) {
    this->connection = connection;
}
PassportMapper::~PassportMapper() {

}



Passport PassportMapper::createPassport(Passport& givenPassport) {
    SQLHSTMT statement;
    Passport passport;
    const wchar_t* query = L"Insert Into passport(number,issuance,given_by) values(?,?,?) returning id";

    SQLRETURN returnCode = SQLAllocHandle(
        SQL_HANDLE_STMT,
        this->connection->connection,
        &statement
    );

    returnCode = SQLPrepare(
        statement,
        (SQLWCHAR*)query,
        SQL_NTS
    );

    SQLBindParameter(
        statement, 1,
        SQL_PARAM_INPUT, SQL_C_LONG,
        SQL_INTEGER, 0, 0, &givenPassport.number, sizeof(givenPassport.number), nullptr
    );
    SQLBindParameter(
        statement, 2,
        SQL_PARAM_INPUT, SQL_C_WCHAR,
        SQL_VARCHAR, 255, 0, (SQLWCHAR*)givenPassport.issuance.c_str(), sizeof(givenPassport.issuance), nullptr
    );
    SQLBindParameter(
        statement, 3,
        SQL_PARAM_INPUT, SQL_C_WCHAR,
        SQL_VARCHAR, 255, 0, (SQLWCHAR*)givenPassport.givenBy.c_str(), sizeof(givenPassport.givenBy), nullptr
    );

    returnCode = SQLExecute(statement);

    SQLFetch(statement);

    SQLGetData(statement, 1, SQL_C_ULONG, &givenPassport.id, 0, NULL);
    std::wcout << "Retrieved: " << givenPassport.id << std::endl;

    if (returnCode != SQL_SUCCESS) {
        showError(SQL_HANDLE_STMT, statement);
        return passport;
    }

    passport = Passport(givenPassport.id, givenPassport.number, givenPassport.issuance, givenPassport.givenBy);
    SQLFreeHandle(SQL_HANDLE_STMT, statement);
    return passport;

}

void PassportMapper::deletePassport(Passport& passportToDelete) {
    SQLHSTMT statement;

    SQLRETURN returnCode = SQLAllocHandle(
        SQL_HANDLE_STMT,
        this->connection->connection,
        &statement
    );
    const wchar_t* query = L"delete from passport where id = ?";

    returnCode = SQLPrepare(
        statement,
        (SQLWCHAR*)query,
        SQL_NTS
    );

    SQLBindParameter(
        statement, 1,
        SQL_PARAM_INPUT, SQL_C_LONG,
        SQL_INTEGER, 0, 0, &passportToDelete.id, sizeof(passportToDelete.id), nullptr
    );

    returnCode = SQLExecute(statement);

    if (returnCode != SQL_SUCCESS) {
        showError(SQL_HANDLE_STMT, statement);
        return;
    }

    SQLFreeHandle(SQL_HANDLE_STMT, statement);
    return;
}

void PassportMapper::updatePassport(Passport& givenPassport) {
    SQLHSTMT statement;
    const wchar_t* query = L"Update passport set number = ?, issuance = ?, given_by = ? where id = ?";

    SQLRETURN returnCode = SQLAllocHandle(
        SQL_HANDLE_STMT,
        this->connection->connection,
        &statement
    );

    returnCode = SQLPrepare(
        statement,
        (SQLWCHAR*)query,
        SQL_NTS
    );

    SQLBindParameter(
        statement, 1,
        SQL_PARAM_INPUT, SQL_C_LONG,
        SQL_INTEGER, 0, 0, &givenPassport.number, sizeof(givenPassport.number), nullptr
    );
    SQLBindParameter(
        statement, 2,
        SQL_PARAM_INPUT, SQL_C_WCHAR,
        SQL_VARCHAR, 255, 0, (WCHAR*)givenPassport.issuance.c_str(), sizeof(givenPassport.issuance), nullptr
    );
    SQLBindParameter(
        statement, 3,
        SQL_PARAM_INPUT, SQL_C_WCHAR,
        SQL_VARCHAR, 255, 0, (WCHAR*)givenPassport.givenBy.c_str(), sizeof(givenPassport.givenBy), nullptr
    );
    SQLBindParameter(
        statement, 4,
        SQL_PARAM_INPUT, SQL_C_LONG,
        SQL_INTEGER, 0, 0, &givenPassport.id, sizeof(givenPassport.id), nullptr
    );
    

    if (returnCode != SQL_SUCCESS) {
        showError(SQL_HANDLE_STMT, statement);
        return;
    }
    returnCode = SQLExecute(statement);


    SQLFreeHandle(SQL_HANDLE_STMT, statement);
    return;
}