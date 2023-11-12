#include "ScheduleMapper.h"
#include "ShowError.h"
#include "MaidMapper.h"
ScheduleMapper::ScheduleMapper(SQLConnection* connection) {
    this->connection = connection;
}
ScheduleMapper::~ScheduleMapper() {

}



Schedule ScheduleMapper::createSchedule(Schedule& givenSchedule) {
    SQLHSTMT statement;
    Schedule scheduleToReturn;
   

  

    const wchar_t* query = L"Insert Into schedule(day,time,maid_id,room_id) values(?,?,?,?) returning id";

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
        SQL_VARCHAR, 255, 0, (WCHAR*)givenSchedule.day_of_work.c_str(), sizeof(givenSchedule.day_of_work), nullptr
    );
    SQLBindParameter(
        statement, 2,
        SQL_PARAM_INPUT, SQL_C_WCHAR,
        SQL_VARCHAR, 255, 0, (WCHAR*)givenSchedule.time_of_work.c_str(), sizeof(givenSchedule.time_of_work), nullptr
    );
    SQLBindParameter(
        statement, 3,
        SQL_PARAM_INPUT, SQL_C_LONG,
        SQL_INTEGER, 0, 0, &givenSchedule.maid_id, sizeof(givenSchedule.maid_id), nullptr
    );
    SQLBindParameter(
        statement, 4,
        SQL_PARAM_INPUT, SQL_C_LONG,
        SQL_INTEGER, 0, 0, &givenSchedule.room_id, sizeof(givenSchedule.room_id), nullptr
    );
   
    returnCode = SQLExecute(statement);

    SQLFetch(statement);

    SQLGetData(statement, 1, SQL_C_ULONG, &givenSchedule.id, 0, NULL);
  

    if (returnCode != SQL_SUCCESS) {
        showError(SQL_HANDLE_STMT, statement);
        wcout << returnCode << endl;
        wstring str;
        wcin >> str;
        return scheduleToReturn;
    }
    scheduleToReturn = Schedule(givenSchedule.id,givenSchedule.day_of_work,givenSchedule.time_of_work,givenSchedule.maid_id,givenSchedule.room_id);
   
    SQLFreeHandle(SQL_HANDLE_STMT, statement);

    return scheduleToReturn;

}

void ScheduleMapper::deleteSchedule(Schedule& givenSchedule) {
    SQLHSTMT statement;
   
    SQLRETURN returnCode = SQLAllocHandle(
        SQL_HANDLE_STMT,
        this->connection->connection,
        &statement
    );
    const wchar_t* query = L"delete from schedule where id = ?";

    returnCode = SQLPrepare(
        statement,
        (SQLWCHAR*)query,
        SQL_NTS
    );

    SQLBindParameter(
        statement, 1,
        SQL_PARAM_INPUT, SQL_C_LONG,
        SQL_INTEGER, 0, 0, &givenSchedule.id, sizeof(givenSchedule.id), nullptr
    );

    returnCode = SQLExecute(statement);

    if (returnCode != SQL_SUCCESS) {
        showError(SQL_HANDLE_STMT, statement);
        return;
    }

   
    SQLFreeHandle(SQL_HANDLE_STMT, statement);
    return;
}

void ScheduleMapper::deleteScheduleByMaidId(int id) {
    SQLHSTMT statement;

    SQLRETURN returnCode = SQLAllocHandle(
        SQL_HANDLE_STMT,
        this->connection->connection,
        &statement
    );
    const wchar_t* query = L"delete from schedule where maid_id = ?";

    returnCode = SQLPrepare(
        statement,
        (SQLWCHAR*)query,
        SQL_NTS
    );

    SQLBindParameter(
        statement, 1,
        SQL_PARAM_INPUT, SQL_C_LONG,
        SQL_INTEGER, 0, 0, &id, sizeof(id), nullptr
    );

    returnCode = SQLExecute(statement);

    if (returnCode != SQL_SUCCESS) {
        showError(SQL_HANDLE_STMT, statement);
        return;
    }

   
    SQLFreeHandle(SQL_HANDLE_STMT, statement);
    return;
}


void ScheduleMapper::updateSchedule(Schedule& givenSchedule) {
    SQLHSTMT statement;
    const wchar_t* query = L"Update schedule set day = ?, time = ? where id = ?";

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
        SQL_VARCHAR, 255, 0, (WCHAR*)givenSchedule.day_of_work.c_str(), sizeof(givenSchedule.day_of_work), nullptr
    );
    SQLBindParameter(
        statement, 2,
        SQL_PARAM_INPUT, SQL_C_WCHAR,
        SQL_VARCHAR, 0, 0, (WCHAR*)givenSchedule.time_of_work.c_str(), sizeof(givenSchedule.time_of_work), nullptr
    );
    SQLBindParameter(
        statement, 3,
        SQL_PARAM_INPUT, SQL_C_LONG,
        SQL_INTEGER, 0, 0, &givenSchedule.id, sizeof(givenSchedule.id), nullptr
    );

    if (returnCode != SQL_SUCCESS) {
        showError(SQL_HANDLE_STMT, statement);
        return;
    }
    returnCode = SQLExecute(statement);


    SQLFreeHandle(SQL_HANDLE_STMT, statement);
    return;
}

//Schedule ScheduleMapper::findById(int schedule_id) {
//    SQLHSTMT statement;
//    Schedule schedule;
//    const wchar_t* query = L"Select id,day,time From schedule where id = ?";
//
//    SQLRETURN returnCode = SQLAllocHandle(
//        SQL_HANDLE_STMT,
//        this->connection->connection,
//        &statement
//    );
//
//    returnCode = SQLPrepare(
//        statement,
//        (SQLWCHAR*)query,
//        SQL_NTS
//    );
//
//    SQLBindParameter(
//        statement, 1,
//        SQL_PARAM_INPUT, SQL_C_LONG,
//        SQL_INTEGER, 0, 0, &schedule_id, sizeof(schedule_id), nullptr
//    );
//
//    if (returnCode != SQL_SUCCESS) {
//        showError(SQL_HANDLE_STMT, statement);
//        return schedule;
//    }
//    returnCode = SQLExecute(statement);
//
//    std::wstring day;
//    std::wstring time;
//    int id = 0;
//
//    while (SQL_SUCCESS == SQLFetch(statement)) {
//        SQLGetData(statement, 1, SQL_C_ULONG, &id, 0, NULL);
//        SQLGetData(statement, 2, SQL_C_WCHAR, &day, 255, NULL);
//        SQLGetData(statement, 3, SQL_C_WCHAR, &time, 255, NULL);
//    }
//    schedule = Schedule(id, day, time);
//
//
//    SQLFreeHandle(SQL_HANDLE_STMT, statement);
//    return schedule;
//}

std::vector<Schedule> ScheduleMapper::getAll() {
    SQLHSTMT statement;
    std::vector<Schedule> scheduleVector;
    const wchar_t* query = L"select schedule.id,schedule.day, schedule.time,schedule.maid_id,schedule.room_id from schedule;";

    SQLRETURN returnCode = SQLAllocHandle(
        SQL_HANDLE_STMT,
        this->connection->connection,
        &statement
    );


    returnCode = SQLExecDirect(statement, (SQLWCHAR*)query, SQL_NTS);

    if (returnCode != SQL_SUCCESS) {
        showError(SQL_HANDLE_STMT, statement);
        return scheduleVector;
    }

    wchar_t day[255];
    wchar_t time[255];

    int id = -1;
    int maid_id =-1;
    int room_id = -1;
   

    

    while (SQL_SUCCESS == SQLFetch(statement)) {
        SQLGetData(statement, 1, SQL_C_ULONG, &id, 0, NULL);
        SQLGetData(statement, 2, SQL_C_WCHAR, &day, 255, NULL);
        SQLGetData(statement, 3, SQL_C_WCHAR, &time, 255, NULL);
        SQLGetData(statement, 4, SQL_C_LONG, &maid_id, 0, NULL);
        SQLGetData(statement, 5, SQL_C_LONG, &room_id, 0, NULL);
      
        Schedule schedule = Schedule(id, (WCHAR*)day, (WCHAR*)time,maid_id,room_id);
        scheduleVector.emplace_back(schedule);
    }

    SQLFreeHandle(SQL_HANDLE_STMT, statement);
    return scheduleVector;
}