#include "AddressMapper.h"
#include "PassportMapper.h"
//#include "ScheduleMapper.h"

#include "MaidMapper.h"
#include "ShowError.h"

MaidMapper::MaidMapper(SQLConnection* connection) {
    this->connection = connection;
}
MaidMapper::~MaidMapper() {

}

Maid MaidMapper::createMaid(Maid& givenMaid) {
    SQLHSTMT statement;
    Maid maid;
    AddressMapper addressMapper = AddressMapper(this->connection);
    //ScheduleMapper scheduleMapper = ScheduleMapper(this->connection);
    connection->startTransaction();
    givenMaid.address = addressMapper.createAddress(givenMaid.address);
   // scheduleMapper.deleteSchedule(givenMaid);
    //givenMaid.schedule = scheduleMapper.createSchedule(givenMaid.schedule);
    const wchar_t* query = L"Insert Into maid(name,surname,patronymic,address_id) values(?,?,?,?) returning id";

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
        SQL_VARCHAR, 255, 0, (WCHAR*)givenMaid.name.c_str(), sizeof(givenMaid.name), nullptr
    );
    SQLBindParameter(
        statement, 2,
        SQL_PARAM_INPUT, SQL_C_WCHAR,
        SQL_VARCHAR, 255, 0, (SQLWCHAR*)givenMaid.surname.c_str(), sizeof(givenMaid.surname), nullptr
    );
    SQLBindParameter(
        statement, 3,
        SQL_PARAM_INPUT, SQL_C_WCHAR,
        SQL_VARCHAR, 255, 0, (SQLWCHAR*)givenMaid.patronymic.c_str(), sizeof(givenMaid.patronymic), nullptr
    );
    SQLBindParameter(
        statement, 4,
        SQL_PARAM_INPUT, SQL_C_LONG,
        SQL_INTEGER, 0, 0, &givenMaid.address.id, sizeof(givenMaid.address.id), nullptr
    );
    SQLBindParameter(
        statement, 5,
        SQL_PARAM_INPUT, SQL_C_LONG,
        SQL_INTEGER, 0, 0, &givenMaid.id, sizeof(givenMaid.id), nullptr
    );

    returnCode = SQLExecute(statement);

    SQLFetch(statement);

    SQLGetData(statement, 1, SQL_C_ULONG, &givenMaid.id, 0, NULL);
    if (returnCode != SQL_SUCCESS) {
        showError(SQL_HANDLE_STMT, statement);
        connection->rollbackTransaction();
        return maid;
    }
    maid = Maid(givenMaid.id, givenMaid.name, givenMaid.surname, givenMaid.patronymic, givenMaid.address);

    SQLFreeHandle(SQL_HANDLE_STMT, statement);
    connection->commitTransaction();
    return maid;

}

void MaidMapper::deleteMaid(Maid& givenMaid) {
    SQLHSTMT statement;
    connection->startTransaction();
    SQLRETURN returnCode = SQLAllocHandle(
        SQL_HANDLE_STMT,
        this->connection->connection,
        &statement
    );
    const wchar_t* query = L"delete from maid where id = ?";

    returnCode = SQLPrepare(
        statement,
        (SQLWCHAR*)query,
        SQL_NTS
    );

    SQLBindParameter(
        statement, 1,
        SQL_PARAM_INPUT, SQL_C_ULONG,
        SQL_INTEGER, 0, 0, &givenMaid.id, 0, nullptr
    );

    returnCode = SQLExecute(statement);
    if (returnCode != SQL_SUCCESS) {
        showError(SQL_HANDLE_STMT, statement);
        connection->rollbackTransaction();
        return;
    }
    AddressMapper addressMapper = AddressMapper(this->connection);
    addressMapper.deleteAddress(givenMaid.address);



    SQLFreeHandle(SQL_HANDLE_STMT, statement);
    connection->commitTransaction();
    return;
}

void MaidMapper::updateMaid(Maid& givenMaid) {
    SQLHSTMT statement;

    AddressMapper addressMapper = AddressMapper(this->connection);
    connection->startTransaction();
    addressMapper.updateAddress(givenMaid.address);
    const wchar_t* query = L"Update maid set name = ?, surname = ?,patronymic = ?, address_id = ? where id = ?";

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
        SQL_VARCHAR, 255, 0, (WCHAR*)givenMaid.name.c_str(), sizeof(givenMaid.name), nullptr
    );
    SQLBindParameter(
        statement, 2,
        SQL_PARAM_INPUT, SQL_C_WCHAR,
        SQL_VARCHAR, 255, 0, (WCHAR*)givenMaid.surname.c_str(), sizeof(givenMaid.surname), nullptr
    );
    SQLBindParameter(
        statement, 3,
        SQL_PARAM_INPUT, SQL_C_WCHAR,
        SQL_VARCHAR, 255, 0, (WCHAR*)givenMaid.patronymic.c_str(), sizeof(givenMaid.patronymic), nullptr
    );
    SQLBindParameter(
        statement, 4,
        SQL_PARAM_INPUT, SQL_C_LONG,
        SQL_INTEGER, 0, 0, &givenMaid.address.id, sizeof(givenMaid.address.id), nullptr
    );
    SQLBindParameter(
        statement, 5,
        SQL_PARAM_INPUT, SQL_C_LONG,
        SQL_INTEGER, 0, 0, &givenMaid.id, sizeof(givenMaid.id), nullptr
    );

    if (returnCode != SQL_SUCCESS) {
        showError(SQL_HANDLE_STMT, statement);
        connection->rollbackTransaction();
        return;
    }
    returnCode = SQLExecute(statement);


    SQLFreeHandle(SQL_HANDLE_STMT, statement);
    connection->commitTransaction();
    return;
}

std::vector<Maid> MaidMapper::getAll() {
    SQLHSTMT statement;
    std::vector<Maid> maidVector;
    const wchar_t* query = L"select maid.id,maid.name,maid.surname,patronymic,address1.id,address1.country,address1.city,address1.street,address1.building from maid INNER JOIN address1 ON address1.id = maid.address_id;";

    SQLRETURN returnCode = SQLAllocHandle(
        SQL_HANDLE_STMT,
        this->connection->connection,
        &statement
    );


    returnCode = SQLExecDirect(statement, (SQLWCHAR*)query, SQL_NTS);

    if (returnCode != SQL_SUCCESS) {
        showError(SQL_HANDLE_STMT, statement);
        return maidVector;
    }

    wchar_t name[255];
    wchar_t surname[255];
    wchar_t patronymic[255];
    wchar_t country[255];
    wchar_t city[255];
    wchar_t street[255];
    int building = 0;
    int id = 0;
    int address_id = 0;


    while (SQL_SUCCESS == SQLFetch(statement)) {
        SQLGetData(statement, 1, SQL_C_LONG, &id, 0, NULL);
        SQLGetData(statement, 2, SQL_C_WCHAR, &name, 255, NULL);
        SQLGetData(statement, 3, SQL_C_WCHAR, &surname, 255, NULL);
        SQLGetData(statement, 4, SQL_C_WCHAR, &patronymic, 255, NULL);
        SQLGetData(statement, 5, SQL_C_LONG, &address_id, 0, NULL);
        SQLGetData(statement, 6, SQL_C_WCHAR, &country, 255, NULL);
        SQLGetData(statement, 7, SQL_C_WCHAR, &city, 255, NULL);
        SQLGetData(statement, 8, SQL_C_WCHAR, &street, 255, NULL);
        SQLGetData(statement, 9, SQL_C_LONG, &building, 0, NULL);
        Address address = Address(address_id, country, city, street, building);
        Maid maid = Maid(id, name, surname, patronymic, address);
        maidVector.emplace_back(maid);
    }

    SQLFreeHandle(SQL_HANDLE_STMT, statement);
    return maidVector;
}