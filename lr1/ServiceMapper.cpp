#include "ServiceMapper.h"

#include "ShowError.h"

ServiceMapper::ServiceMapper(SQLConnection* connection) {
    this->connection = connection;
}
ServiceMapper::~ServiceMapper() {

}

Service ServiceMapper::createService(Service& givenService, int guest_id) {
    SQLHSTMT statement;
    Service service;

    const wchar_t* query = L"Insert Into service(name) values(?) returning id";

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
        SQL_PARAM_INPUT, SQL_C_WCHAR,
        SQL_VARCHAR, 255, 0, (WCHAR*)givenService.service_name.c_str(), sizeof(givenService.service_name), nullptr
    );

    returnCode = SQLExecute(statement);

    SQLFetch(statement);

    SQLGetData(statement, 1, SQL_C_ULONG, &givenService.id, 0, NULL);
    if (returnCode != SQL_SUCCESS) {
        showError(SQL_HANDLE_STMT, statement);
        return service;
    }
    service = Service(givenService.id, givenService.service_name,guest_id);

    SQLFreeHandle(SQL_HANDLE_STMT, statement);

    SQLHSTMT statement1;

    const wchar_t* query1 = L"Insert Into guest_service(guest_id,service_id) values(?,?)";

    SQLRETURN returnCode1 = SQLAllocHandle(
        SQL_HANDLE_STMT,
        this->connection->connection,
        &statement1
    );

    returnCode1 = SQLPrepare(
        statement,
        (SQLWCHAR*)query1,
        SQL_NTS
    );

    SQLBindParameter(
        statement1, 1,
        SQL_PARAM_INPUT, SQL_C_LONG,
        SQL_INTEGER, 0, 0, &guest_id, sizeof(guest_id), nullptr
    );
    SQLBindParameter(
        statement1, 2,
        SQL_PARAM_INPUT, SQL_C_LONG,
        SQL_INTEGER, 0, 0, &givenService.id, sizeof(givenService.id), nullptr
    );

    returnCode1 = SQLExecute(statement1);

    if (returnCode1 != SQL_SUCCESS) {
        showError(SQL_HANDLE_STMT, statement1);
        return service;
    }

    service = Service(givenService.id, givenService.service_name, guest_id);

    SQLFreeHandle(SQL_HANDLE_STMT, statement1);

    return service;
}

void ServiceMapper::deleteService(Service& givenService) {

    SQLHSTMT statement1;

    SQLRETURN returnCode1 = SQLAllocHandle(
        SQL_HANDLE_STMT,
        this->connection->connection,
        &statement1
    );

    const wchar_t* query1 = L"delete from guest_service where service_id = ?";

    returnCode1 = SQLPrepare(
        statement1,
        (SQLWCHAR*)query1,
        SQL_NTS
    );

    SQLBindParameter(
        statement1, 1,
        SQL_PARAM_INPUT, SQL_C_ULONG,
        SQL_INTEGER, 0, 0, &givenService.id, 0, nullptr
    );

    returnCode1 = SQLExecute(statement1);
    if (returnCode1 != SQL_SUCCESS) {
        showError(SQL_HANDLE_STMT, statement1);
        return;
    }
    SQLFreeHandle(SQL_HANDLE_STMT, statement1);




    SQLHSTMT statement;

    SQLRETURN returnCode = SQLAllocHandle(
        SQL_HANDLE_STMT,
        this->connection->connection,
        &statement
    );

    const wchar_t* query = L"delete from service where id = ?";

    returnCode = SQLPrepare(
        statement,
        (SQLWCHAR*)query,
        SQL_NTS
    );

    SQLBindParameter(
        statement, 1,
        SQL_PARAM_INPUT, SQL_C_ULONG,
        SQL_INTEGER, 0, 0, &givenService.id, 0, nullptr
    );

    returnCode = SQLExecute(statement);
    if (returnCode != SQL_SUCCESS) {
        showError(SQL_HANDLE_STMT, statement);
        wcout << returnCode;
        wstring str;
        wcin >> str;
        return;
    }
    SQLFreeHandle(SQL_HANDLE_STMT, statement);
    
    return;
}



void ServiceMapper::deleteServiceByGuestId(int id) {
    SQLHSTMT statement;

    SQLRETURN returnCode = SQLAllocHandle(
        SQL_HANDLE_STMT,
        this->connection->connection,
        &statement
    );
    const wchar_t* query = L"delete from guest_service where guest_id = ?";

    returnCode = SQLPrepare(
        statement,
        (SQLWCHAR*)query,
        SQL_NTS
    );

    SQLBindParameter(
        statement, 1,
        SQL_PARAM_INPUT, SQL_C_ULONG,
        SQL_INTEGER, 0, 0, &id, 0, nullptr
    );

    returnCode = SQLExecute(statement);
    if (returnCode != SQL_SUCCESS) {
        showError(SQL_HANDLE_STMT, statement);
        return;
    }
    SQLFreeHandle(SQL_HANDLE_STMT, statement);
    return;
}

std::vector<Service> ServiceMapper::getAll() {
    SQLHSTMT statement;
    std::vector<Service> serviceVector;
    const wchar_t* query = L"select guest_service.guest_id,guest_service.service_id, service.name from guest_service INNER JOIN service ON service.id = guest_service.service_id";

    SQLRETURN returnCode = SQLAllocHandle(
        SQL_HANDLE_STMT,
        this->connection->connection,
        &statement
    );


    returnCode = SQLExecDirect(statement, (SQLWCHAR*)query, SQL_NTS);

    if (returnCode != SQL_SUCCESS) {
        showError(SQL_HANDLE_STMT, statement);
        return serviceVector;
    }
    int service_id = 0;
    wchar_t service_name[255];
    int guest_id = 0;



    while (SQL_SUCCESS == SQLFetch(statement)) {
        SQLGetData(statement, 1, SQL_C_LONG, &guest_id, 0, NULL);
        SQLGetData(statement, 2, SQL_C_LONG, &service_id, 0, NULL);
        SQLGetData(statement, 3, SQL_C_WCHAR, &service_name, 255, NULL);
        Service service = Service(service_id, service_name,guest_id);
        serviceVector.emplace_back(service);
    }

    SQLFreeHandle(SQL_HANDLE_STMT, statement);
    return serviceVector;
}