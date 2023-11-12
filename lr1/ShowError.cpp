#include "ShowError.h"

#include "iostream"

void showError(unsigned int handletype, const SQLHANDLE& handle) {
    SQLWCHAR sqlstate[1024];
    SQLWCHAR message[1024];
    SQLRETURN returnCode = SQLGetDiagRec(handletype, handle, 1, sqlstate, NULL, message, 1024, NULL);
    if (returnCode != SQL_SUCCESS) return;

    std::wcout << "Message: " << message << "\nSQLSTATE: " << sqlstate << std::endl;
}