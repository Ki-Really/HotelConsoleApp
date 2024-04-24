#include "Mapper.h"
#include "Guest.h"
#include "string.h"
#include "Room.h"
#include "Address.h"
#include "Passport.h"
#include "AddressMapper.h"
#include "RoomMapper.h"
#include "PassportMapper.h"
#include "ShowError.h"
#include "ServiceMapper.h"

Mapper::Mapper(SQLConnection* connection) {
	this->connection = connection;
}

Mapper::~Mapper() {

}

Guest Mapper::createGuest(Guest& givenGuest) {

    AddressMapper addressMapper = AddressMapper(this->connection);
    PassportMapper passportMapper = PassportMapper(this->connection);
    RoomMapper roomMapper = RoomMapper(this->connection);
    Guest guest;
    
    connection->startTransaction();

    givenGuest.address = addressMapper.createAddress(givenGuest.address);
    givenGuest.room = roomMapper.findRoomByNumber(givenGuest.room.number);
    givenGuest.passport = passportMapper.createPassport(givenGuest.passport);


    SQLHSTMT statement;
    const wchar_t* query = L"Insert Into guest(name,surname,patronymic,gender,birth_date,address_id,passport_id,room_id,"
        "parking_lot_number,auto_number,date_of_entry,departure_date) values(?,?,?,?,?,?,?,?,?,?,?,?) returning id";

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
        SQL_VARCHAR, 255, 0, (SQLWCHAR*)givenGuest.name.c_str(), sizeof(givenGuest.name), nullptr
    );
    SQLBindParameter(
        statement, 2,
        SQL_PARAM_INPUT, SQL_C_WCHAR,
        SQL_VARCHAR, 255, 0, (SQLWCHAR*)givenGuest.surname.c_str(), sizeof(givenGuest.surname), nullptr
    );
    SQLBindParameter(
        statement, 3,
        SQL_PARAM_INPUT, SQL_C_WCHAR,
        SQL_VARCHAR, 255, 0, (SQLWCHAR*)givenGuest.patronymic.c_str(), sizeof(givenGuest.patronymic), nullptr
    );
    SQLBindParameter(
        statement, 4,
        SQL_PARAM_INPUT, SQL_C_WCHAR,
        SQL_VARCHAR, 255, 0, (SQLWCHAR*)givenGuest.gender.c_str(), sizeof(givenGuest.gender), nullptr
    );

    SQLBindParameter(
        statement, 5,
        SQL_PARAM_INPUT, SQL_C_WCHAR,
        SQL_VARCHAR, 255, 0, (SQLWCHAR*)givenGuest.birthdate.c_str(), sizeof(givenGuest.birthdate), nullptr
    );

    SQLBindParameter(
        statement, 6,
        SQL_PARAM_INPUT, SQL_C_LONG,
        SQL_INTEGER, 0, 0, &givenGuest.address.id, sizeof(givenGuest.address.id), nullptr
    );

    SQLBindParameter(
        statement, 7,
        SQL_PARAM_INPUT, SQL_C_LONG,
        SQL_INTEGER, 0, 0, &givenGuest.passport.id, sizeof(givenGuest.passport.id), nullptr
    );

    SQLBindParameter(
        statement, 8,
        SQL_PARAM_INPUT, SQL_C_LONG,
        SQL_INTEGER, 0, 0, &givenGuest.room.id, sizeof(guest.room.id), nullptr
    );

    SQLBindParameter(
        statement, 9,
        SQL_PARAM_INPUT, SQL_C_LONG,
        SQL_INTEGER, 0, 0, &givenGuest.parking_lot_number, sizeof(givenGuest.parking_lot_number), nullptr
    );
    SQLBindParameter(
        statement, 10,
        SQL_PARAM_INPUT, SQL_C_WCHAR,
        SQL_VARCHAR, 0, 0, (WCHAR*)givenGuest.auto_number.c_str(), sizeof(givenGuest.auto_number), nullptr
    );
    SQLBindParameter(
        statement, 11,
        SQL_PARAM_INPUT, SQL_C_WCHAR,
        SQL_VARCHAR, 255, 0, (SQLWCHAR*)givenGuest.date_of_entry.c_str(), sizeof(givenGuest.date_of_entry), nullptr
    );
    SQLBindParameter(
        statement, 12,
        SQL_PARAM_INPUT, SQL_C_WCHAR,
        SQL_VARCHAR, 255, 0, (SQLWCHAR*)givenGuest.departure_date.c_str(), sizeof(givenGuest.departure_date), nullptr
    );

    returnCode = SQLExecute(statement);
    
    int id = 0;
    SQLFetch(statement);

    SQLGetData(statement, 1, SQL_C_LONG, &id, 0, NULL);
    if (returnCode != SQL_SUCCESS) {
        showError(SQL_HANDLE_STMT, statement);
       connection->rollbackTransaction();
        return guest;
    }
    guest = Guest(id, givenGuest.name, givenGuest.surname, givenGuest.patronymic,
        givenGuest.gender, givenGuest.birthdate, givenGuest.address, givenGuest.passport,
        givenGuest.room, givenGuest.parking_lot_number, givenGuest.auto_number, givenGuest.date_of_entry, givenGuest.departure_date);

    SQLFreeHandle(SQL_HANDLE_STMT, statement);
   
   connection->commitTransaction();
    return guest;
}

void Mapper::deleteGuest(Guest& givenGuest){
    SQLHSTMT statement;
    AddressMapper addressMapper = AddressMapper(this->connection);
    PassportMapper passportMapper = PassportMapper(this->connection);
    ServiceMapper serviceMapper = ServiceMapper(this->connection);
   
    std::vector<Service> serviceVector = serviceMapper.getAll();
    connection->startTransaction();
    for (int i = 0; i < serviceVector.size(); i++) {
        
        if (serviceVector[i].guest_id == givenGuest.id) {
            serviceMapper.deleteService(serviceVector[i]);
        }
        
    }

    SQLRETURN returnCode = SQLAllocHandle(
        SQL_HANDLE_STMT,
        this->connection->connection,
        &statement
    );
    const wchar_t* query = L"delete from guest where id = ?";

    returnCode = SQLPrepare(
        statement,
        (SQLWCHAR*)query,
        SQL_NTS
    );

    SQLBindParameter(
        statement, 1,
        SQL_PARAM_INPUT, SQL_C_LONG,
        SQL_INTEGER, 0, 0, &givenGuest.id, sizeof(givenGuest.id), nullptr
    );

    returnCode = SQLExecute(statement);
    addressMapper.deleteAddress(givenGuest.address);
    passportMapper.deletePassport(givenGuest.passport);
    if (returnCode != SQL_SUCCESS) {
        showError(SQL_HANDLE_STMT, statement);
        connection->rollbackTransaction();
        return;
    }

    SQLFreeHandle(SQL_HANDLE_STMT, statement);

    connection->commitTransaction();
    return;
}

void Mapper::updateGuest(Guest& givenGuest) {
    SQLHSTMT statement;

    PassportMapper passportMapper = PassportMapper(this->connection);
    AddressMapper addressMapper = AddressMapper(this->connection);

    connection->startTransaction();
    addressMapper.updateAddress(givenGuest.address);

    passportMapper.updatePassport(givenGuest.passport);

    const wchar_t* query = L"update guest set name = ?, surname = ?, patronymic = ?, gender = ?,birth_date = ?, address_id = ?, passport_id = ?, room_id = ?, parking_lot_number = ?, auto_number = ?, date_of_entry = ?, departure_date = ? where id = ?";

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
        SQL_VARCHAR, 255, 0, (WCHAR*)givenGuest.name.c_str(), sizeof(givenGuest.name), nullptr
    );
    SQLBindParameter(
        statement, 2,
        SQL_PARAM_INPUT, SQL_C_WCHAR,
        SQL_VARCHAR, 255, 0, (WCHAR*)givenGuest.surname.c_str(), sizeof(givenGuest.surname), nullptr
    );
    SQLBindParameter(
        statement, 3,
        SQL_PARAM_INPUT, SQL_C_WCHAR,
        SQL_VARCHAR, 255, 0, (WCHAR*)givenGuest.patronymic.c_str(), sizeof(givenGuest.patronymic), nullptr
    );
    SQLBindParameter(
        statement, 4,
        SQL_PARAM_INPUT, SQL_C_WCHAR,
        SQL_VARCHAR, 255, 0, (WCHAR*)givenGuest.gender.c_str(), sizeof(givenGuest.gender), nullptr
    );
    SQLBindParameter(
        statement, 5,
        SQL_PARAM_INPUT, SQL_C_WCHAR,
        SQL_VARCHAR, 255, 0, (WCHAR*)givenGuest.birthdate.c_str(), sizeof(givenGuest.birthdate), nullptr
    );
    SQLBindParameter(
        statement, 6,
        SQL_PARAM_INPUT, SQL_C_LONG,
        SQL_INTEGER, 0, 0, &givenGuest.address.id, sizeof(givenGuest.address.id), nullptr
    );
    SQLBindParameter(
        statement, 7,
        SQL_PARAM_INPUT, SQL_C_LONG,
        SQL_INTEGER, 0, 0, &givenGuest.passport.id, sizeof(givenGuest.passport.id), nullptr
    );
    SQLBindParameter(
        statement, 8,
        SQL_PARAM_INPUT, SQL_C_LONG,
        SQL_INTEGER, 0, 0, &givenGuest.room.id, sizeof(givenGuest.room.id), nullptr
    );
    SQLBindParameter(
        statement, 9,
        SQL_PARAM_INPUT, SQL_C_LONG,
        SQL_INTEGER, 0, 0, &givenGuest.parking_lot_number, sizeof(givenGuest.parking_lot_number), nullptr
    );
    SQLBindParameter(
        statement, 10,
        SQL_PARAM_INPUT, SQL_C_WCHAR,
        SQL_VARCHAR, 255, 0, (WCHAR*)givenGuest.auto_number.c_str(), sizeof(givenGuest.auto_number), nullptr
    );
    SQLBindParameter(
        statement, 11,
        SQL_PARAM_INPUT, SQL_C_WCHAR,
        SQL_VARCHAR, 255, 0, (WCHAR*)givenGuest.date_of_entry.c_str(), sizeof(givenGuest.date_of_entry), nullptr
    );
    SQLBindParameter(
        statement, 12,
        SQL_PARAM_INPUT, SQL_C_WCHAR,
        SQL_VARCHAR, 255, 0, (WCHAR*)givenGuest.departure_date.c_str(), sizeof(givenGuest.departure_date), nullptr
    );
    SQLBindParameter(
        statement, 13,
        SQL_PARAM_INPUT, SQL_C_LONG,
        SQL_INTEGER, 0, 0,&givenGuest.id, sizeof(givenGuest.id), nullptr
    );

    if (returnCode != SQL_SUCCESS) {
        connection->rollbackTransaction();
        showError(SQL_HANDLE_STMT, statement);
        return;
    }
    returnCode = SQLExecute(statement);


    SQLFreeHandle(SQL_HANDLE_STMT, statement);
    connection->commitTransaction();
    return;
}

std::vector<Guest> Mapper::getAll() {
    SQLHSTMT statement;
    std::vector<Guest> guestVector;
    const wchar_t* query = L"select guest.id,guest.name,guest.surname,guest.patronymic,guest.gender,guest.birth_date,guest.parking_lot_number,guest.auto_number,guest.date_of_entry,guest.departure_date,address1.id,address1.country,address1.city,address1.street,address1.building,passport.id, passport.number,passport.issuance,passport.given_by,room_id,room.number,room.people_count from guest INNER JOIN address1 ON address1.id = guest.address_id Inner join passport ON passport.id = guest.passport_id inner join room ON room.id = guest.room_id;";

    SQLRETURN returnCode = SQLAllocHandle(
        SQL_HANDLE_STMT,
        this->connection->connection,
        &statement
    );


    returnCode = SQLExecDirect(statement, (SQLWCHAR*)query, SQL_NTS);

    if (returnCode != SQL_SUCCESS) {
        showError(SQL_HANDLE_STMT, statement);
        return guestVector;
    }
    int id = 0;
    wchar_t name[255];
    wchar_t surname[255];
    wchar_t patronymic[255];
    wchar_t gender[255];
    wchar_t birth_date[255];
    int parking_lot = 0;
    wchar_t auto_number[255];
    wchar_t date_e[255];
    wchar_t date_d[255];

    int address_id = 0;
    wchar_t country[255];
    wchar_t city[255];
    wchar_t street[255];
    int building = 0;

    int passport_id = 0;
    int passport_number = 0;
    wchar_t issuance[255];
    wchar_t givenBy[255];

    int room_id = 0;
    int room_number = 0;
    int people_count = 0;

    while (SQL_SUCCESS == SQLFetch(statement)) {
        SQLGetData(statement, 1, SQL_C_LONG, &id, 0, NULL);
        SQLGetData(statement, 2, SQL_C_WCHAR, &name, 255, NULL);
        SQLGetData(statement, 3, SQL_C_WCHAR, &surname, 255, NULL);
        SQLGetData(statement, 4, SQL_C_WCHAR, &patronymic, 255, NULL);
        SQLGetData(statement, 5, SQL_C_WCHAR, &gender, 255, NULL);
        SQLGetData(statement, 6, SQL_C_WCHAR, &birth_date, 255, NULL);
        SQLGetData(statement, 7, SQL_C_LONG, &parking_lot, 0, NULL);
        SQLGetData(statement, 8, SQL_C_WCHAR, &auto_number, 255, NULL);
        SQLGetData(statement, 9, SQL_C_WCHAR, &date_e, 255, NULL);
        SQLGetData(statement, 10, SQL_C_WCHAR, &date_d, 255, NULL);

        SQLGetData(statement, 11, SQL_C_LONG, &address_id, 0, NULL);
        SQLGetData(statement, 12, SQL_C_WCHAR, &country, 255, NULL);
        SQLGetData(statement, 13, SQL_C_WCHAR, &city, 255, NULL);
        SQLGetData(statement, 14, SQL_C_WCHAR, &street, 255, NULL);
        SQLGetData(statement, 15, SQL_C_LONG, &building, 0, NULL);

        SQLGetData(statement, 16, SQL_C_LONG, &passport_id, 0, NULL);
        SQLGetData(statement, 17, SQL_C_LONG, &passport_number, 0, NULL);
        SQLGetData(statement, 18, SQL_C_WCHAR, &issuance, 255, NULL);
        SQLGetData(statement, 19, SQL_C_WCHAR, &givenBy, 255, NULL);

        SQLGetData(statement, 20, SQL_C_LONG, &room_id, 0, NULL);
        SQLGetData(statement, 21, SQL_C_LONG, &room_number, 0, NULL);
        SQLGetData(statement, 22, SQL_C_LONG, &people_count, 0, NULL);

        Address address = Address(address_id, country, city, street, building);
        Passport passport = Passport(passport_id, passport_number, issuance, givenBy);
        Room room = Room(room_id, room_number, people_count);
        Guest guest = Guest(id, name, surname, patronymic, gender, birth_date, address, passport, room, parking_lot, auto_number, date_e, date_d);
        guestVector.emplace_back(guest);
    }

    SQLFreeHandle(SQL_HANDLE_STMT, statement);
    return guestVector;
}

