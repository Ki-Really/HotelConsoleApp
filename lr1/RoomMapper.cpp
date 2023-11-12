#include "RoomMapper.h"
#include "ShowError.h"
//#include "Mapper.h"

RoomMapper::RoomMapper(SQLConnection* connection) {
	this->connection = connection;
}
RoomMapper::~RoomMapper() {

}

void RoomMapper::createRoom(Room& givenRoom) {
    SQLHSTMT statement;
    Room room;
    const wchar_t* query = L"Insert Into room(number,people_count) values(?,?)";

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
        SQL_INTEGER, 0, 0, &givenRoom.number, sizeof(givenRoom.number), nullptr
    );
    SQLBindParameter(
        statement, 2,
        SQL_PARAM_INPUT, SQL_C_LONG,
        SQL_INTEGER,0, 0, &givenRoom.people_count, sizeof(givenRoom.people_count), nullptr
    );

    returnCode = SQLExecute(statement);

    if (returnCode != SQL_SUCCESS) {
        showError(SQL_HANDLE_STMT, statement);
        return;
    }

    SQLFreeHandle(SQL_HANDLE_STMT, statement);
    return;
}
//
//
Room RoomMapper::findRoomByNumber(int& room_number) {
    SQLHSTMT statement;
    Room room;
    const wchar_t* query = L"Select id,number,people_count From room where number = ?";
    
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
        SQL_INTEGER, 0, 0, &room_number, sizeof(room_number), nullptr
    );

    if (returnCode != SQL_SUCCESS) {
        showError(SQL_HANDLE_STMT, statement);
        return room;
    }
    returnCode = SQLExecute(statement);

    int number = 0;
    int people_count= 0;
    int id = 0;

    while (SQL_SUCCESS == SQLFetch(statement)) {
        SQLGetData(statement, 1, SQL_C_ULONG, &id, 0, NULL);
        SQLGetData(statement, 2, SQL_C_ULONG, &number, 0, NULL);
        SQLGetData(statement, 3, SQL_C_ULONG, &people_count, 0, NULL);
       // std::wcout << "Retrieved: " << number << " " << people_count << std::endl;
    }
    room = Room(id, number, people_count);


    SQLFreeHandle(SQL_HANDLE_STMT, statement);
    return room;
}

void RoomMapper::deleteRoom(int& room_number) {
    
    SQLHSTMT statement;
    /*Mapper mapper = Mapper(connection);
    mapper.deleteGuest()*/

    SQLRETURN returnCode = SQLAllocHandle(
        SQL_HANDLE_STMT,
        this->connection->connection,
        &statement
    );
    const wchar_t* query = L"delete from room where number = ?";

    returnCode = SQLPrepare(
        statement,
        (SQLWCHAR*)query,
        SQL_NTS
    );

    SQLBindParameter(
        statement, 1,
        SQL_PARAM_INPUT, SQL_C_LONG,
        SQL_INTEGER, 0, 0, &room_number, sizeof(room_number), nullptr
    );

    returnCode = SQLExecute(statement);

    if (returnCode != SQL_SUCCESS) {
        showError(SQL_HANDLE_STMT, statement);
        return;
    }

    SQLFreeHandle(SQL_HANDLE_STMT, statement);
    return;

}

void RoomMapper::updateRoom(Room& roomToUpdate) {
    SQLHSTMT statement;
    const wchar_t* query = L"Update room set people_count = ? where number = ?";

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
        SQL_INTEGER, 0, 0, &roomToUpdate.people_count, sizeof(roomToUpdate.people_count), nullptr
    );
    SQLBindParameter(
        statement, 2,
        SQL_PARAM_INPUT, SQL_C_LONG,
        SQL_INTEGER, 0, 0, &roomToUpdate.number, sizeof(roomToUpdate.number), nullptr
    );

    if (returnCode != SQL_SUCCESS) {
        showError(SQL_HANDLE_STMT, statement);
        return;
    }
    returnCode = SQLExecute(statement);


    SQLFreeHandle(SQL_HANDLE_STMT, statement);
    return;
}

std::vector<Room> RoomMapper::getAll() {
    SQLHSTMT statement;
    std::vector<Room> roomVector;
    const wchar_t* query = L"select id,number, people_count from room;";

    SQLRETURN returnCode = SQLAllocHandle(
        SQL_HANDLE_STMT,
        this->connection->connection,
        &statement
    );

   
    returnCode = SQLExecDirect(statement,(SQLWCHAR*)query,SQL_NTS);

    if (returnCode != SQL_SUCCESS) {
        showError(SQL_HANDLE_STMT, statement);
        return roomVector;
    }

    int number = 0;
    int people_count = 0;
    int id = 0;
    

    while (SQL_SUCCESS == SQLFetch(statement)) {
        SQLGetData(statement, 1, SQL_C_ULONG, &id, 0, NULL);
        SQLGetData(statement, 2, SQL_C_ULONG, &number, 0, NULL);
        SQLGetData(statement, 3, SQL_C_ULONG, &people_count, 0, NULL);
        Room room = Room(id,number,people_count);
        roomVector.emplace_back(room);
    }

    SQLFreeHandle(SQL_HANDLE_STMT, statement);
    return roomVector;
}