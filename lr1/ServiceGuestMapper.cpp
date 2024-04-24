#include "ServiceMapper.h"
#include "ServiceGuestMapper.h"
#include "ShowError.h"
#include "ServiceGuest.h"

ServiceGuestMapper::ServiceGuestMapper(SQLConnection* connection) {
    this->connection = connection;
}
ServiceGuestMapper::~ServiceGuestMapper() {

}

void ServiceGuestMapper::createServiceGuest(ServiceGuest givenServiceGuest) {
    SQLHSTMT statement;
    ServiceGuest serviceGuest;

    const wchar_t* query = L"Insert Into guest_service(guest_id,service_id) values(?,?)";

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
        SQL_INTEGER, 255, 0, &givenServiceGuest.guest_id, sizeof(givenServiceGuest.guest_id), nullptr
    );
    SQLBindParameter(
        statement, 2,
        SQL_PARAM_INPUT, SQL_C_LONG,
        SQL_INTEGER, 0, 0, &givenServiceGuest.service_id, sizeof(givenServiceGuest.service_id), nullptr
    );

    returnCode = SQLExecute(statement);

    SQLFreeHandle(SQL_HANDLE_STMT, statement);
    return;
}

void ServiceGuestMapper::deleteServiceGuest(int service_id) {
    SQLHSTMT statement;

    SQLRETURN returnCode = SQLAllocHandle(
        SQL_HANDLE_STMT,
        this->connection->connection,
        &statement
    );
    const wchar_t* query = L"delete from guest_service where service_id = ?";

    returnCode = SQLPrepare(
        statement,
        (SQLWCHAR*)query,
        SQL_NTS
    );

    SQLBindParameter(
        statement, 1,
        SQL_PARAM_INPUT, SQL_C_ULONG,
        SQL_INTEGER, 0, 0, &service_id, 0, nullptr
    );

    returnCode = SQLExecute(statement);
    if (returnCode != SQL_SUCCESS) {
        showError(SQL_HANDLE_STMT, statement);
        return;
    }
    SQLFreeHandle(SQL_HANDLE_STMT, statement);
    return;
}



std::vector<ServiceGuest> ServiceGuestMapper::getAll() {
    SQLHSTMT statement;
    std::vector<ServiceGuest> serviceGuestVector;
    const wchar_t* query = L"select guest_id, service_id from guest_service;";

    SQLRETURN returnCode = SQLAllocHandle(
        SQL_HANDLE_STMT,
        this->connection->connection,
        &statement
    );


    returnCode = SQLExecDirect(statement, (SQLWCHAR*)query, SQL_NTS);

    if (returnCode != SQL_SUCCESS) {
        showError(SQL_HANDLE_STMT, statement);
        return serviceGuestVector;
    }
    int service_id = 0;
    int guest_id = 0;



    while (SQL_SUCCESS == SQLFetch(statement)) {
        SQLGetData(statement, 1, SQL_C_LONG, &guest_id, 0, NULL);
        SQLGetData(statement, 2, SQL_C_LONG, &service_id, 0, NULL);
        ServiceGuest serviceGuest = ServiceGuest(service_id, guest_id);
        serviceGuestVector.emplace_back(serviceGuest);
    }

    SQLFreeHandle(SQL_HANDLE_STMT, statement);
    return serviceGuestVector;
}