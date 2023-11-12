#include "Migrations.h"

Migrations::Migrations(SQLConnection* connection) {
    this->connection = connection;
}

void Migrations::runMigrations() {
    SQLHSTMT statement;

    SQLRETURN returnCode = SQLAllocHandle(
        SQL_HANDLE_STMT,
        this->connection->connection,
        &statement
    );

        const wchar_t* query = L"BEGIN"
            
            "create type if not exists day_of_week as enum ("
            "'Понедельник', 'Вторник', 'Среда', 'Четверг', 'Пятница', 'Суббота', 'Воскресенье'"
            ");"

            "create type if not exists gender as enum ("
            "'Мужчина', 'Женщина'"
            ");"

            "CREATE TABLE if not exists 'address1'("
            "'id' serial primary key NOT NULL,"
            "'country' VARCHAR(255) NOT NULL,"
            "'city' VARCHAR(255) NOT NULL, "
            "'street' VARCHAR(255) NOT NULL,"
            "'building' VARCHAR(255) NOT NULL"
            ");"

            "CREATE TABLE if not exists 'maid'("
            "'id' serial primary key NOT NULL,"
            "'name' VARCHAR(255) NOT NULL, "
            "'surname' VARCHAR(255) NOT NULL,"
            "'patronymic' VARCHAR(255) NOT NULL, "
            "'address_id' int REFERENCES address1(id) NOT NULL"
            ");"

            "CREATE TABLE if not exists 'schedule'("
            "'id' serial primary key NOT NULL,"
            "'day'  day_of_week NOT NULL,"
            "'time' time without time zone NOT NULL,"
            "'maid_id' int REFERENCES maid(id) NOT NULL"
            ");"

            "CREATE TABLE if not exists 'passport'("
            "'id' serial primary key NOT NULL,"
            "'number' INTEGER NOT NULL unique, "
            "'issuance' DATE NOT NULL,"
            "'given_by' VARCHAR(255) NOT NULL"
            ");"
            "CREATE TABLE if not exists 'room'("
            "'id' serial primary key NOT NULL, "
            "'number' INTEGER NOT NULL unique, "
            "'people_count' INTEGER NOT NULL"
            ");"

            "CREATE TABLE if not exists 'guest'("
            "'id' serial primary key NOT NULL,"
            "'name' VARCHAR(255) NOT NULL,"
            "'surname' VARCHAR(255) NOT NULL, "
            "'patronymic' VARCHAR(255) NOT NULL,"
            "'gender' gender NOT NULL,"
            "'birth_date' DATE NOT NULL,"
            "'address_id' int REFERENCES address1(id)  NOT NULL,"
            "'passport_id' int REFERENCES passport(id) NOT NULL,"
            "'room_id' int REFERENCES room(id) NOT NULL,"
            "'parking_lot_number' INTEGER NULL unique,"
            "'auto_number' VARCHAR NULL, "
            "'date_of_entry' DATE NOT NULL,"
            "'departure_date' DATE NOT NULL"
            ");"


            "CREATE TABLE if not exists 'service'("
            "'id' serial primary key NOT NULL, "
            "'name' VARCHAR(255) NOT NULL,"
            "'guest_id' int REFERENCES guest(id) NOT NULL"
            ");"
            "COMMIT;";

   SQLExecDirect(statement, (SQLWCHAR*)query, SQL_NTS);
   SQLFreeHandle(SQL_HANDLE_STMT, statement);
}





