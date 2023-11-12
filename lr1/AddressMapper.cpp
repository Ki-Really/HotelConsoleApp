#include "AddressMapper.h"
#include "ShowError.h"

AddressMapper::AddressMapper(SQLConnection* connection) {
    this->connection = connection;
}
AddressMapper::~AddressMapper() {

}


Address AddressMapper::createAddress(Address& givenAddress) {
    SQLHSTMT statement;
    Address address;

    SQLRETURN returnCode = SQLAllocHandle(
        SQL_HANDLE_STMT,
        this->connection->connection,
        &statement
    );
    const wchar_t* query = L"Insert Into address1(country,city,street,building) values(?,?,?,?) returning id";

    returnCode = SQLPrepare(
        statement,
        (SQLWCHAR*)query,
        SQL_NTS
    );

    SQLBindParameter(
        statement, 1,
        SQL_PARAM_INPUT, SQL_C_WCHAR,
        SQL_VARCHAR, 255, 0, (SQLWCHAR*)givenAddress.country.c_str(), sizeof(givenAddress.country), nullptr
    );
    SQLBindParameter(
        statement, 2,
        SQL_PARAM_INPUT, SQL_C_WCHAR,
        SQL_VARCHAR, 255, 0, (SQLWCHAR*)givenAddress.city.c_str(), sizeof(givenAddress.city), nullptr
    );
    SQLBindParameter(
        statement, 3,
        SQL_PARAM_INPUT, SQL_C_WCHAR,
        SQL_VARCHAR, 255, 0, (SQLWCHAR*)givenAddress.street.c_str(), sizeof(givenAddress.street), nullptr
    );
    SQLBindParameter(
        statement, 4,
        SQL_PARAM_INPUT, SQL_C_LONG,
        SQL_INTEGER, 0, 0, &givenAddress.building, sizeof(givenAddress.building), nullptr
    );

    returnCode = SQLExecute(statement);
    

    SQLFetch(statement);
    SQLGetData(statement, 1, SQL_C_ULONG, &givenAddress.id, 0, NULL);
    std::wcout << "Retrieved: " << givenAddress.id << std::endl;
   

    if (returnCode != SQL_SUCCESS) {
        showError(SQL_HANDLE_STMT, statement);
        return address;
    }

    address = Address(givenAddress.id, givenAddress.country, givenAddress.city, givenAddress.street, givenAddress.building);
    SQLFreeHandle(SQL_HANDLE_STMT, statement);
    return address;


}

void AddressMapper::deleteAddress(Address& addressToDelete) {
    SQLHSTMT statement;

    SQLRETURN returnCode = SQLAllocHandle(
        SQL_HANDLE_STMT,
        this->connection->connection,
        &statement
    );
    const wchar_t* query = L"delete from address1 where id = ?";

    returnCode = SQLPrepare(
        statement,
        (SQLWCHAR*)query,
        SQL_NTS
    );

    SQLBindParameter(
        statement, 1,
        SQL_PARAM_INPUT, SQL_C_LONG,
        SQL_INTEGER, 0, 0, &addressToDelete.id, sizeof(addressToDelete.id), nullptr
    );
    
    returnCode = SQLExecute(statement);

    if (returnCode != SQL_SUCCESS) {
        showError(SQL_HANDLE_STMT, statement);
        return;
    }
   
    SQLFreeHandle(SQL_HANDLE_STMT, statement);
    return;
}

void AddressMapper::updateAddress(Address& givenAddress){
    SQLHSTMT statement;
    const wchar_t* query = L"Update address1 set country = ?, city = ?, street = ?, building = ? where id = ?";

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
        SQL_VARCHAR, 255, 0, (WCHAR*)givenAddress.country.c_str(), sizeof(givenAddress.country), nullptr
    );
    SQLBindParameter(
        statement, 2,
        SQL_PARAM_INPUT, SQL_C_WCHAR,
        SQL_VARCHAR, 255, 0, (WCHAR*)givenAddress.city.c_str(), sizeof(givenAddress.city), nullptr
    );
    SQLBindParameter(
        statement, 3,
        SQL_PARAM_INPUT, SQL_C_WCHAR,
        SQL_VARCHAR, 255, 0, (WCHAR*)givenAddress.street.c_str(), sizeof(givenAddress.street), nullptr
    );
    SQLBindParameter(
        statement, 4,
        SQL_PARAM_INPUT, SQL_C_LONG,
        SQL_INTEGER, 0, 0, &givenAddress.building, sizeof(givenAddress.building), nullptr
    );
    SQLBindParameter(
        statement, 5,
        SQL_PARAM_INPUT, SQL_C_LONG,
        SQL_INTEGER, 0, 0, &givenAddress.id, sizeof(givenAddress.id), nullptr
    );

    if (returnCode != SQL_SUCCESS) {
        showError(SQL_HANDLE_STMT, statement);
        return;
    }
    returnCode = SQLExecute(statement);


    SQLFreeHandle(SQL_HANDLE_STMT, statement);
    return;
}