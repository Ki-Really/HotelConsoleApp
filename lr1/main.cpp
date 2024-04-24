#include <limits>
#include <iostream>
#include <iomanip>
#include <io.h>
#include <fcntl.h>  
#include "SQLConnection.h"
#include "Mapper.h"
#include "Address.h"
#include "Migrations.h"
#include "AddressMapper.h"
#include "PassportMapper.h"
#include "RoomMapper.h"
#include "ScheduleMapper.h"
#include "MaidMapper.h"
#include "Service.h"
#include "ServiceMapper.h"
#include <sstream>
#include"ServiceGuestMapper.h"
#include"ServiceGuest.h"
#include <stdexcept>

using namespace std;

bool isValidDate(const std::wstring& input) {
    if (input.length() != 10) {
        return false;
    }
    if (input[2] != L'-' || input[5] != L'-') {
        return false;
    }

    try {
        
        int day = std::stoi(input.substr(0, 2));
        int month = std::stoi(input.substr(3, 2));
        int year = std::stoi(input.substr(6, 4));

       
        if (day < 1 || day > 31 || month < 1 || month > 12 || year < 0) {
            return false;
        }

      
        if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30) {
            return false;
        }
        else if (month == 2) {
            if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) {
                if (day > 29) {
                    return false;
                }
            }
            else {
                if (day > 28) {
                    return false;
                }
            }
        }
    }
    catch (const std::invalid_argument&) {
        return false;
    }

    return true;
}

void main_menu(){
    _setmode(_fileno(stdout), _O_U16TEXT);
    system("cls"); 
    std::wcout << L"Выберите действие!" << endl;
    std::wcout << L"1. Гости" << endl;
    std::wcout << L"2. Комнаты" << endl;
    std::wcout << L"3. Услуги" << endl;
    std::wcout << L"4. Уборщицы" << endl;
    std::wcout << L"5. График уборки" << endl;
    std::wcout << L"6. Выход" << endl;
    std::wcout << L">";
}
void room_menu_options() {
    setlocale(LC_ALL, "rus_rus.866");

    _setmode(_fileno(stdout), _O_U16TEXT);
    system("cls"); 
    std::wcout << L"Выберите действие!" << endl;
    std::wcout << L"1. Добавить комнату" << endl;
    std::wcout << L"2. Удалить комнату" << endl;
    std::wcout << L"3. Изменить комнату" << endl;
    std::wcout << L"4. Поиск комнаты" << endl;
    std::wcout << L"5. Вывести список всех комнат!" << endl;
    std::wcout << L"6. В главное меню" << endl;
    std::wcout << L"7. Выход" << endl;
    std::wcout << L">";
}

void room_menu_cases(SQLConnection* connection) {
    RoomMapper roomMapper = RoomMapper(connection);
    std::vector<Room> roomVector = roomMapper.getAll();
    Mapper guestMapper = Mapper(connection);
    std::vector<Guest> guestVector = guestMapper.getAll();
    int choice = 0;
    while (choice != 7) {
        room_menu_options();
        while (!(wcin >> choice)) {

            wcin.clear();
            wcin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::wcout << L"Неверный ввод (введите номер пункта)!" << endl;

        }

        switch (choice) {
        case 1: {
            int room_number;
            int people_count;
            std::wcout << L"Введите номер комнаты для добавления!" << endl;
            while (!(wcin >> room_number)) {
                wcin.clear();
                wcin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::wcout << L"Неверный ввод (введите число)!" << endl;
            }
            int flag = 0;
            for (int i = 0; i < roomVector.size(); i++) {
                if (room_number == roomVector[i].number) {
                    wcout << L"Эта комната уже существует. Введите любую клавишу" << endl;
                    wstring str;
                    wcin >> str;
                    flag = 1;
                }

            }
            if (flag == 1) {
                break;
            }
            
            std::wcout << L"Введите количество человек для которых предназначена комната!" << endl;
            while (!(wcin >> people_count)) {
                wcin.clear();
                wcin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::wcout << L"Неверный ввод (введите число)!" << endl;
            }
            Room room = Room(-1, room_number, people_count);
            roomMapper.createRoom(room);
            roomVector.emplace_back(room);
            break;
        }

        case 2: {
            int room_number;
            std::wcout << L"Список всех комнат: " << endl;
            wcout << L"Индекс " << setw(15) << L"Номер комнаты " << setw(15) << L"Рассчитана на N человек " << endl;
            wcout << setfill(L'-') << setw(45) << L"-" << endl;
            wcout << setfill(L' ');
            for (int i = 0; i < roomVector.size(); i++) {
                wcout << i << setw(15) << roomVector[i].number << setw(15) << roomVector[i].people_count << endl;
            }
            std::wcout << L"Введите номер комнаты для ее удаления!" << endl;
            while (!(wcin >> room_number)) {
                wcin.clear();
                wcin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::wcout << L"Неверный ввод (введите число)!" << endl;
            }
            for (int i = 0; i < guestVector.size(); i++) {
                if (guestVector[i].room.number == room_number) {
                    wcout << L"В этой комнате сейчас живут люди! Невозможно удалить. Введите клавишу для возвращения в меню!" << endl;
                    wstring str;
                    wcin >> str;
                    room_menu_cases(connection);
                }
            }
            for (int i = 0; i < roomVector.size(); i++) {
                if (room_number == roomVector[i].number) {
                    
                    roomMapper.deleteRoom(room_number);
                    roomVector = roomMapper.getAll();
                    std::wcout << "Комната успешно удалена!" << endl;
                }
            }
            
            break;
        }
        case 3: {
            int people_count;
            int room_number;
            std::wcout << L"Список всех комнат: " << endl;
            wcout << L"Индекс " << setw(15) << L"Номер комнаты " << setw(15) << L"Рассчитана на N человек " << endl;
            wcout << setfill(L'-') << setw(45) << L"-" << endl;
            wcout << setfill(L' ');
            for (int i = 0; i < roomVector.size(); i++) {
                wcout << i << setw(15) << roomVector[i].number << setw(15) << roomVector[i].people_count << endl;
            }
            std::wcout << L"Введите номер комнаты которую хотите изменить!" << endl;
            while (!(wcin >> room_number)) {
                wcin.clear();
                wcin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::wcout << L"Неверный ввод (введите число)!" << endl;
            }
            for (int i = 0; i < roomVector.size(); i++) {
                if (room_number == roomVector[i].number) {
                    std::wcout << L"Введите новое количество человек для которых предназначена комната!" << endl;
                    while (!(wcin >> people_count)) {
                        wcin.clear();
                        wcin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::wcout << L"Неверный ввод (введите число)!" << endl;
                    }

                    Room room = Room(-1, room_number, people_count);
                    roomMapper.updateRoom(room);
                    roomVector = roomMapper.getAll();
                    break;
                }
            }
           
            break;
        }
        case 4: {
            int room_number;
            wstring wait;
            std::wcout << L"Введите номер комнаты для нахождения!" << endl;
            while (!(wcin >> room_number)) {
                wcin.clear();
                wcin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::wcout << L"Неверный ввод (введите число)!" << endl;
            }
            int flag = -1;
            for (int i = 0; i < roomVector.size(); i++) {
                if (room_number == roomVector[i].number) {
                    Room room = roomMapper.findRoomByNumber(room_number);
                std:wcout << L"Номер комнаты = " << room.number << L", вместимость = " << room.people_count << endl;
                    wcout << L"Введите что нибудь для продолжения" << endl;
                    std::wcin >> wait;
                    break;
                }
            }
           
            break;
        }
        case 5: {
            wstring wait;
            std::wcout << L"Список всех комнат: " << endl;
            wcout << L"Индекс " << setw(15) << L"Номер комнаты " << setw(15) << L"Рассчитана на N человек " << endl;
            wcout << setfill(L'-') << setw(45) << L"-" << endl;
            wcout << setfill(L' ');
            for (int i = 0; i < roomVector.size(); i++) {
                wcout << i << setw(15) << roomVector[i].number << setw(15) << roomVector[i].people_count << endl;
            }
            wcout << L"Введите любой символ для продолжения" << endl;
            std::wcin >> wait;
            
            break;
        }
        case 6: {
            return;
            break;
        }
        case 7: {
            exit(0);
            break;
        }
        }
    }
}

void schedule_menu_options() {
    setlocale(LC_ALL, "rus_rus.866");

    _setmode(_fileno(stdout), _O_U16TEXT);
    system("cls"); 
    std::wcout << L"Выберите действие!" << endl;
    std::wcout << L"1. Добавить расписание" << endl;
    std::wcout << L"2. Удалить расписание" << endl;
    std::wcout << L"3. Изменить расписание" << endl;
    std::wcout << L"4. Вывести весь график" << endl;
    std::wcout << L"5. В главное меню" << endl;
    std::wcout << L"6. Выход" << endl;
    std::wcout << L">";
}

void schedule_menu_cases(SQLConnection* connection) {
    ScheduleMapper scheduleMapper = ScheduleMapper(connection);
    MaidMapper maidMapper = MaidMapper(connection);
    std::vector<Maid> maidVector = maidMapper.getAll();

    RoomMapper roomMapper = RoomMapper(connection);
    std::vector<Room> roomVector = roomMapper.getAll();
    int choice = 0;
    std::vector<Schedule> scheduleVector;
    scheduleVector = scheduleMapper.getAll();
    while (choice != 6) {
        schedule_menu_options(); 
        while (!(wcin >> choice)) {

            wcin.clear();
            wcin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            std::wcout << L"Неверный ввод (введите номер пункта)!" << endl;

        }

        switch (choice) {
        case 1: {
            std::wstring day;
            std::wstring time;
            std::wcout << L"Введите день недели(Понедельник, Вторник, ...)" << endl;
            std::wcin >> day;
            wstring monday = L"Понедельник";
            wstring tuesday = L"Вторник";
            wstring wednesday = L"Среда";
            wstring thursday = L"Четверг";
            wstring friday = L"Пятница";
            wstring saturday = L"Суббота";
            wstring sunday = L"Воскресенье";

            if (day != monday && day != tuesday && day != wednesday && day != thursday && day != friday && day != saturday && day != sunday) {
                wcout << L"Неверный ввод пола, попробуйте заново. Введите любой символ для продолжения" << endl;
                std::wstring str;
                wcin >> str;
                break;
            }
            int maid_id;

            std::wcout << L"Введите время!(12:00)" << endl;
            std::wcin >> time;

            std::wcout << L"Список всех уборщиц: " << endl;
            wcout << L"Индекс " << setw(15) << L"Имя " << setw(15) << L"Фамилия " << setw(15) << L"Отчество " << setw(15) << L"Страна " <<
                L"Город " << setw(15) << L"Улица " << setw(15) << L"Дом " << endl;
            wcout << setfill(L'-') << setw(110) << L"-" << endl;
            wcout << setfill(L' ');
            for (int i = 0; i < maidVector.size(); i++) {
                wcout << i << setw(15) << maidVector[i].name << setw(15) << maidVector[i].surname << setw(15) << maidVector[i].patronymic <<
                    setw(15) << maidVector[i].address.country << setw(15) << maidVector[i].address.city << setw(15) << maidVector[i].address.street <<
                    setw(15) << maidVector[i].address.building << endl;
            }

            std::wcout << L"Введите индекс уборщицы" << endl;
            while (!(wcin >> maid_id)) {
                wcin.clear();
                wcin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::wcout << L"Неверный ввод (введите число)!" << endl;
            }

            int room_id;
            std::wcout << L"Список всех комнат: " << endl;
            wcout << L"Индекс " << setw(15) << L"Номер комнаты " << setw(15) << L"Рассчитана на N человек " << endl;
            wcout << setfill(L'-') << setw(45) << L"-" << endl;
            wcout << setfill(L' ');
            for (int i = 0; i < roomVector.size(); i++) {
                wcout << i << setw(15) << roomVector[i].number << setw(15) << roomVector[i].people_count << endl;
            }
            std::wcout << L"Введите индекс комнаты за которой закреплена уборщица" << endl;
            while (!(wcin >> room_id)) {
                wcin.clear();
                wcin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::wcout << L"Неверный ввод (введите число)!" << endl;
            }
           
            maid_id = maidVector[maid_id].id;
            for (int i = 0; i < roomVector.size(); i++) {
                if (room_id == i) {
                    room_id = roomVector[i].id;
                    break;
                }
            }
            Schedule schedule1;
            Schedule schedule = Schedule(-1, day, time, maid_id, room_id);
            schedule1 = scheduleMapper.createSchedule(schedule);
            scheduleVector.emplace_back(schedule1);
            scheduleVector = scheduleMapper.getAll();
            break;
        }

        case 2: {
            
            int schedule_id;
            std::vector<int> maidIdVector;
            for (int i = 0; i < scheduleVector.size(); i++) {
                for (int j = 0; j < maidVector.size(); j++) {
                    if (maidVector[j].id == scheduleVector[i].maid_id) {
                        maidIdVector.emplace_back(j);
                    }
                }

            }
            std::vector<int> roomIdVector;
            for (int i = 0; i < scheduleVector.size(); i++) {
                for (int j = 0; j < roomVector.size(); j++) {
                    if (roomVector[j].id == scheduleVector[i].room_id) {
                        roomIdVector.emplace_back(j);
                    }
                }

            }
            std::wcout << L"Список всех расписаний: " << endl;
            wcout << L"Индекс " << setw(15) << L"День недели " << setw(15) << L"Время начала уборки " << setw(15) << L"Индекс уборщицы " <<
                setw(15) << L"Индекс комнаты " << endl;
            wcout << setfill(L'-') << setw(45) << L"-" << endl;
            wcout << setfill(L' ');
            for (int i = 0; i < scheduleVector.size(); i++) {
                wcout << i << setw(15) << scheduleVector[i].day_of_work << setw(15) << scheduleVector[i].time_of_work << setw(15) << maidIdVector[i] <<
                    setw(15) << roomIdVector[i] << endl;
            }
            std::wcout << L"Введите номер расписания в таблице расписаний для удаления!" << endl;
            while (!(wcin >> schedule_id)) {
                wcin.clear();
                wcin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::wcout << L"Неверный ввод (введите число)!" << endl;
            }
            for (int i = 0; i < scheduleVector.size(); i++) {
                if (schedule_id == i) {
                    for (int i = 0; i < scheduleVector.size(); i++) {
                        if (schedule_id == i) {
                           
                            scheduleMapper.deleteSchedule(scheduleVector[schedule_id]);
                            scheduleVector = scheduleMapper.getAll();

                        }
                    }
                    break;
                }
            }
            std::wcout << "График успешно удален!" << endl;
            break;
        }
        case 3: {
            int schedule_id;
            wstring updatedDay;
            wstring updatedTime;
            int id;
            
            std::vector<int> maidIdVector;
            for (int i = 0; i < scheduleVector.size(); i++) {
                for (int j = 0; j < maidVector.size(); j++) {
                    if (maidVector[j].id == scheduleVector[i].maid_id) {
                        maidIdVector.emplace_back(j);
                    }
                }

            }
            std::vector<int> roomIdVector;
            for (int i = 0; i < scheduleVector.size(); i++) {
                for (int j = 0; j < roomVector.size(); j++) {
                    if (roomVector[j].id == scheduleVector[i].room_id) {
                        roomIdVector.emplace_back(j);
                    }
                }

            }
            std::wcout << L"Список всех расписаний: " << endl;
            wcout << L"Индекс " << setw(15) << L"День недели " << setw(15) << L"Время начала уборки " << setw(15) << L"Индекс уборщицы " <<
                setw(15) << L"Индекс комнаты " << endl;
            wcout << setfill(L'-') << setw(45) << L"-" << endl;
            wcout << setfill(L' ');
            for (int i = 0; i < scheduleVector.size(); i++) {
                wcout << i << setw(15) << scheduleVector[i].day_of_work << setw(15) << scheduleVector[i].time_of_work << setw(15) << maidIdVector[i] <<
                    setw(15) << roomIdVector[i] << endl;
            }
            std::wcout << L"Введите id расписания для изменения!" << endl;
            while (!(wcin >> id)) {
                wcin.clear();
                wcin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::wcout << L"Неверный ввод (введите число)!" << endl;
            }
            for (int i = 0; i < scheduleVector.size(); i++) {
                if (id == i) {
                    for (int i = 0; i < scheduleVector.size(); i++) {
                        if (id == i) {
                            id = scheduleVector[i].id;
                        }
                    }
                    std::wcout << L"Введите новый день!" << endl;
                    std::wcin >> updatedDay;
                    wstring monday = L"Понедельник";
                    wstring tuesday = L"Вторник";
                    wstring wednesday = L"Среда";
                    wstring thursday = L"Четверг";
                    wstring friday = L"Пятница";
                    wstring saturday = L"Суббота";
                    wstring sunday = L"Воскресенье";

                    if (updatedDay != monday && updatedDay != tuesday && updatedDay != wednesday && updatedDay != thursday && updatedDay != friday && updatedDay != saturday && updatedDay != sunday) {
                        wcout << L"Неверный ввод пола, попробуйте заново. Введите любой символ для продолжения" << endl;
                        std::wstring str;
                        wcin >> str;
                        break;
                    }
                    std::wcout << L"Введите новое время(12:00, 13:45, ...)!" << endl;
                    std::wcin >> updatedTime;
                   
                    Schedule schedule = Schedule(id, updatedDay, updatedTime, -1,-1);

                    scheduleMapper.updateSchedule(schedule);
                    scheduleVector = scheduleMapper.getAll();
                    break;
                }
            }

            break;
        }
        case 4: {
            
            std::vector<int> maidIdVector;
            for (int i = 0; i < scheduleVector.size(); i++) {
                for (int j = 0; j < maidVector.size(); j++) {
                    if (maidVector[j].id == scheduleVector[i].maid_id) {
                        maidIdVector.emplace_back(j);
                    }
                } 
            }
            std::vector<int> roomIdVector;
            for (int i = 0; i < scheduleVector.size(); i++) {
                for (int j = 0; j < roomVector.size(); j++) {
                    if (roomVector[j].id == scheduleVector[i].room_id) {
                        roomIdVector.emplace_back(j);
                    }
                }
            }
            std::wcout << L"Список всех расписаний: " << endl;
            wcout << L"Индекс " << setw(15) << L"День недели " << setw(15) << L"Время начала уборки " << setw(15) << L"Индекс уборщицы " << 
                setw(15) << L"Индекс комнаты " << endl;
            wcout << setfill(L'-') << setw(45) << L"-" << endl;
            wcout << setfill(L' ');
            for (int i = 0; i < scheduleVector.size(); i++) {
                wcout << i << setw(15) << scheduleVector[i].day_of_work << setw(15) << scheduleVector[i].time_of_work << setw(15) <<maidIdVector[i] << 
                    setw(15) << roomIdVector[i] << endl;
            }

            wstring wait;
            wcout << L"Введите что нибудь для продолжения!" << endl;
            std::wcin >> wait;
            break;
        }
        case 5: {
            return;
            break;
        }
        case 6: {
            exit(0);
            break;
        }
        }

    }
}

void maid_menu_options() {
    setlocale(LC_ALL, "rus_rus.866");

    _setmode(_fileno(stdout), _O_U16TEXT);
    system("cls"); 
    std::wcout << L"Выберите действие!" << endl;
    std::wcout << L"1. Добавить уборщицу" << endl;
    std::wcout << L"2. Удалить уборщицу" << endl;
    std::wcout << L"3. Изменить данные уборщицы" << endl;
    std::wcout << L"4. Вывести данные об уборщицах" << endl;
    std::wcout << L"5. В главное меню" << endl;
    std::wcout << L"6. Выход" << endl;
    std::wcout << L">";
}

void maid_menu_cases(SQLConnection* connection) {
    MaidMapper* maidMapper = new MaidMapper(connection);
    AddressMapper* addressMapper = new AddressMapper(connection);
    int choice = 0;
    std::vector<Maid> maidVector = maidMapper->getAll();
    ScheduleMapper scheduleMapper = ScheduleMapper(connection);
    std::vector<Schedule> scheduleVector = scheduleMapper.getAll();
    
    while (choice != 6) {
        maid_menu_options(); 
        while (!(wcin >> choice)) {

            wcin.clear();
            wcin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');


            std::wcout << L"Неверный ввод (введите номер пункта)!" << endl;

        }

        switch (choice) {
        case 1: {
            int wait;
            std::wstring name;
            std::wstring surname;
            std::wstring patronymic;
            int building;
            std::wstring country;
            std::wstring city;
            std::wstring street;
            std::wcout << L"Введите имя" << endl;
            std::wcin >> name;
            std::wcout << L"Введите фамилию" << endl;
            std::wcin >> surname;
            std::wcout << L"Введите отчество" << endl;
            std::wcin >> patronymic;
            std::wcout << L"Введите страну" << endl;
            std::wcin >> country;
            std::wcout << L"Введите город" << endl;
            std::wcin >> city;
            std::wcout << L"Введите улицу"<<endl;
            std::wcin >> street;
            std::wcout << L"Введите номер дома" << endl;
            while (!(wcin >> building)) {
                wcin.clear();
                wcin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::wcout << L"Неверный ввод (введите число)!" << endl;
            }

            Address address(-1,country,city,street,building);
           
            Maid maid = Maid(-1, name, surname, patronymic, address);
            Maid maidToPreserve = maidMapper->createMaid(maid);

            maidVector.emplace_back(maidToPreserve);
                 
            break;
        }

        case 2: {
            int maid_id;
            int wait;
            std::wcout << L"Список всех уборщиц: " << endl;
            wcout << L"Индекс " << setw(15) << L"Имя " << setw(15) << L"Фамилия " << setw(15) << L"Отчество " << setw(15) << L"Страна " <<
                L"Город " << setw(15) << L"Улица " << setw(15) << L"Дом " << endl;
            wcout << setfill(L'-') << setw(110) << L"-" << endl;
            wcout << setfill(L' ');
            for (int i = 0; i < maidVector.size(); i++) {
                wcout << i << setw(15) << maidVector[i].name << setw(15) << maidVector[i].surname << setw(15) << maidVector[i].patronymic <<
                    setw(15) << maidVector[i].address.country << setw(15) << maidVector[i].address.city << setw(15) << maidVector[i].address.street <<
                    setw(15) << maidVector[i].address.building << endl;
            }
            std::wcout << L"Введите id уборщицы для удаления!" << endl;
            while (!(wcin >> maid_id)) {
                wcin.clear();
                wcin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::wcout << L"Неверный ввод (введите число)!" << endl;
            }
            
            for (int i = 0; i < maidVector.size(); i++) {
                if (maid_id == i) {
                    scheduleMapper.deleteScheduleByMaidId(maidVector[maid_id].id);
                    maidMapper->deleteMaid(maidVector[maid_id]);
                    maidVector = maidMapper->getAll();
                    break;
                }
            }
            break;
        }
        case 3: {
            wstring updatedName;
            wstring updatedSurname;
            wstring updatedPatronymic;
            wstring updatedCountry;
            wstring updatedCity;
            wstring updatedStreet;
            int updatedBuilding;
            int writeId;
            int addressId;
            std::wcout << L"Список всех уборщиц: " << endl;
            wcout << L"Индекс " << setw(15) << L"Имя " << setw(15) << L"Фамилия " << setw(15) << L"Отчество " << setw(15) << L"Страна " <<
                L"Город " << setw(15) << L"Улица " << setw(15) << L"Дом " << endl;
            wcout << setfill(L'-') << setw(110) << L"-" << endl;
            wcout << setfill(L' ');
            for (int i = 0; i < maidVector.size(); i++) {
                wcout << i << setw(15) << maidVector[i].name << setw(15) << maidVector[i].surname << setw(15) << maidVector[i].patronymic <<
                    setw(15) << maidVector[i].address.country << setw(15) << maidVector[i].address.city << setw(15) << maidVector[i].address.street <<
                    setw(15) << maidVector[i].address.building << endl;
            }
            std::wcout << L"Введите id уборщицы для изменения!" << endl;
            while (!(wcin >> writeId)) {
                wcin.clear();
                wcin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::wcout << L"Неверный ввод (введите число)!" << endl;
            }
            for (int i = 0; i < maidVector.size(); i++) {
                if (writeId == i) {
                    for (int i = 0; i < maidVector.size(); i++) {
                        if (writeId == i) {
                            writeId = maidVector[i].id;
                            addressId = maidVector[i].address.id;
                        }
                    }
                    
                    std::wcout << L"Введите имя" << endl;
                    std::wcin >> updatedName;
                    std::wcout << L"Введите фамилию" << endl;
                    std::wcin >> updatedSurname;
                    std::wcout << L"Введите отчество" << endl;
                    std::wcin >> updatedPatronymic;
                    std::wcout << L"Введите страну" << endl;
                    std::wcin >> updatedCountry;
                    std::wcout << L"Введите город" << endl;
                    std::wcin >> updatedCity;
                    std::wcout << L"Введите улицу" << endl;
                    std::wcin >> updatedStreet;
                    std::wcout << L"Введите номер дома" << endl;
                    while (!(wcin >> updatedBuilding)) {
                        wcin.clear();
                        wcin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::wcout << L"Неверный ввод (введите число)!" << endl;
                    }

                    Address updatedAddress(addressId, updatedCountry, updatedCity, updatedStreet, updatedBuilding);

                    Maid updatedMaid = Maid(writeId, updatedName, updatedSurname, updatedPatronymic, updatedAddress);
                    maidMapper->updateMaid(updatedMaid);
                    maidVector = maidMapper->getAll();
                    break;
                }
            }
           
            break;
        }
        case 4: {
            std::wcout << L"Список всех уборщиц: " << endl;
            wcout << L"Индекс " << setw(15) << L"Имя " << setw(15) << L"Фамилия " << setw(15) << L"Отчество " << setw(15) << L"Страна " <<
                L"Город " << setw(15) << L"Улица " << setw(15) << L"Дом " <<endl;
            wcout << setfill(L'-') << setw(110) << L"-" << endl;
            wcout << setfill(L' ');
            for (int i = 0; i < maidVector.size(); i++) {
                wcout << i << setw(15) << maidVector[i].name << setw(15) << maidVector[i].surname << setw(15) << maidVector[i].patronymic <<
                    setw(15) << maidVector[i].address.country << setw(15) << maidVector[i].address.city << setw(15) << maidVector[i].address.street <<
                    setw(15) << maidVector[i].address.building << endl;
            }
            wcout << L"Введите что нибудь для продолжения!" << endl;
            wstring wait;
            std::wcin >> wait;
            break;
        }
        case 5: {
            return;
            break;
        }
        case 6: {
            exit(0);
            break;
        }
        }

    }
}


void guest_menu_options() {
    setlocale(LC_ALL, "rus_rus.866");

    _setmode(_fileno(stdout), _O_U16TEXT);
    system("cls"); 
    std::wcout << L"Выберите действие!" << endl;
    std::wcout << L"1. Добавить гостя" << endl;
    std::wcout << L"2. Удалить гостя" << endl;
    std::wcout << L"3. Изменить данные гостя" << endl;
    std::wcout << L"4. Вывести данные о госте" << endl;
    std::wcout << L"5. В главное меню" << endl;
    std::wcout << L"6. Выход" << endl;
    std::wcout << L">";
}

void guest_menu_cases(SQLConnection* connection) {
    Mapper* guestMapper = new Mapper(connection);
    AddressMapper* addressMapper = new AddressMapper(connection);
    RoomMapper* roomMapper = new RoomMapper(connection);
    std::vector<Room> roomVector = roomMapper->getAll();
    int choice = 0;
    std::vector<Guest> guestVector = guestMapper->getAll();

   
    while (choice != 6) {
        guest_menu_options(); 
        while (!(wcin >> choice)) {

            wcin.clear();
            wcin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');


            std::wcout << L"Неверный ввод (введите номер пункта)!" << endl;

        }

        switch (choice) {
        case 1: {
            int wait;
            std::wstring name;
            std::wstring surname;
            std::wstring patronymic;
            std::wstring gender;
            std::wstring birth_date;
            std::wstring date_e;
            std::wstring date_d;
            int parking_lot;
            std::wstring auto_number;

            int building;
            std::wstring country;
            std::wstring city;
            std::wstring street;

            int passport_number;
            std::wstring issuance;
            std::wstring givenBy;
            
            int room_number;
            int people_count;
            std::wcout << L"Введите имя"<<endl;
            std::wcin >> name;
            std::wcout << L"Введите фамилию"<< endl;
            std::wcin >> surname;
            std::wcout << L"Введите отчество" << endl;
            std::wcin >> patronymic;
            std::wcout << L"Введите пол(Женщина, Мужчина)" << endl;
            std::wcin >> gender;
            wstring male = L"Мужчина";
            wstring female = L"Женщина";
            if (male != gender && female != gender) {
                wcout << L"Неверный ввод пола, попробуйте заново. Введите любой символ для продолжения" << endl;
                std::wstring str;
                wcin >> str;
                break;
            }
           
            std::wcout << L"Введите пол дату рождения (XX-XX-XXXX)" << endl;
            std::wcin >> birth_date;
            bool isValidBD = isValidDate(birth_date);
            if (isValidBD == 0) {
                wstring str;
                wcout << L"Неверно введена дата рождения. Нажмите что нибудь для продолжения" << endl;
                wcin >> str;
                break;
            }

            std::wcout << L"Введите номер парковочного места" << endl;
            while (!(wcin >> parking_lot)) {
                wcin.clear();
                wcin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::wcout << L"Неверный ввод (введите число)!" << endl;
            }
            for (int i = 0; i < guestVector.size(); i++) {
                if (guestVector[i].parking_lot_number == parking_lot) {
                    wcout << L"Такой номер парковочного места уже существует. Введите что нибудь для продолжения" << endl;
                    wstring str;
                    wcin >> str;
                    guest_menu_cases(connection);
                }
            }
            std::wcout << L"Введите номер автомобиля" << endl;
            std::wcin >> auto_number;
            std::wcout << L"Введите дату прибытия (XX-XX-XXXX)" << endl;
            std::wcin >> date_e;
            bool isValidDateE = isValidDate(date_e);
            if (isValidDateE == 0) {
                wstring str;
                wcout << L"Неверно введена дата. Нажмите что нибудь для продолжения" << endl;
                wcin >> str;
                break;
            }
            std::wcout << L"Введите дату уезда (XX-XX-XXXX)" << endl;
            std::wcin >> date_d;
            bool isValidDateD = isValidDate(date_d);
            if (isValidDateD == 0) {
                wstring str;
                wcout << L"Неверно введена дата. Нажмите что нибудь для продолжения" << endl;
                wcin >> str;
                break;
            }

            std::wcout << L"Введите страну" << endl;
            std::wcin >> country;
            std::wcout << L"Введите город" << endl;
            std::wcin >> city;
            std::wcout << L"Введите улицу" << endl;
            std::wcin >> street;
            std::wcout << L"Введите номер дома" << endl;
            while (!(wcin >> building)) {
                wcin.clear();
                wcin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::wcout << L"Неверный ввод (введите число)!" << endl;
            }

            std::wcout << L"Введите номер пасспорта" << endl;
            
            while (!(wcin >> passport_number)) {
                wcin.clear();
                wcin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::wcout << L"Неверный ввод (введите число)!" << endl;
            }
            for (int i = 0; i < guestVector.size(); i++) {
                if (guestVector[i].passport.number == passport_number) {
                    wcout << L"Такой номер паспорта уже существует. Введите что нибудь для продолжения" << endl;
                    wstring str;
                    wcin >> str;
                    guest_menu_cases(connection);
                }
            }
            
            std::wcout << L"Введите дату выдачи паспорта (XX-XX-XXXX)" << endl;
            std::wcin >> issuance;
            bool isValidIssuance = isValidDate(issuance);
            if (isValidIssuance == 0) {
                wstring str;
                wcout << L"Неверно введена дата. Нажмите что нибудь для продолжения" << endl;
                wcin >> str;
                break;
            }
            std::wcout << L"Введите кем выдан пасспорт" << endl;
            std::wcin >> givenBy;

            std::wcout << L"Введите номер комнаты заселения" << endl;
            while (!(wcin >> room_number)) {
                wcin.clear();
                wcin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::wcout << L"Неверный ввод (введите число)!" << endl;
            }
            int flag = -1;
            
            while (flag != 1) {
                for (int i = 0; i < roomVector.size(); i++) {
                    if (room_number == roomVector[i].number) {
                        flag = 1;
                        room_number = roomVector[i].number;
                        Address address = Address(-1, country, city, street, building);
                        Room room = Room(-1, room_number, 0);
                        Passport passport = Passport(-1, passport_number, issuance, givenBy);
                        Guest guest = Guest(-1, name, surname, patronymic, gender, birth_date, address, passport, room, parking_lot, auto_number, date_e, date_d);
                        guestMapper->createGuest(guest);
                        guestVector.emplace_back(guest);
                        break;
                    }
                    else { flag = 0; }
                }
                if (flag == 1) {
                    break;
                }
                else {
                    flag = 0;
                    wcout << L"Введите номер комнаты которая существует!" << endl;
                    wcin >> room_number;
                }
                
            }

            break;
        }

        case 2: {
            int guest_id;
            int wait;
            ServiceMapper serviceMapper = ServiceMapper(connection);

            std::wcout << L"Список всех гостей: " << endl;
            guestVector = guestMapper->getAll();
            for (int i = 0; i < guestVector.size(); i++) {
                std::wcout << L"Индекс " << i << L"; " << L"Имя " << guestVector[i].name << L"; " << L"Фамилия " << guestVector[i].surname << L"; " << L"Отчество "
                    << guestVector[i].patronymic << L"; " << L"Пол " << guestVector[i].gender <<
                    L"; " << L"Дата рождения " << guestVector[i].birthdate <<
                    L"; " << L"Страна " << guestVector[i].address.country << L"; " << L"Город " << guestVector[i].address.city << L"; " << L"Улица "
                    << guestVector[i].address.street
                    << L"; " << L"Дом " << guestVector[i].address.building << L"; " << L"Номер паспорта " << guestVector[i].passport.number << L"; "
                    << L"Дата выдачи паспорта " << guestVector[i].passport.issuance << L"; " << L"Номер комнаты " <<
                    guestVector[i].room.number << L"; " << L"Количество людей в комнате " << guestVector[i].room.people_count << L"; "
                    << L"Номер парк. места " << guestVector[i].parking_lot_number << L"; " << L"Номер авто " <<
                    guestVector[i].auto_number << L"; " << L"Дата приезда " << guestVector[i].date_of_entry << L"; " << L"Дата уезда "
                    << guestVector[i].departure_date << endl;
                wcout << endl;
                wcout << endl;
            }
            std::wcout << L"Введите id готся для удаления!" << endl;
            while (!(wcin >> guest_id)) {
                wcin.clear();
                wcin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::wcout << L"Неверный ввод (введите число)!" << endl;
            }
            for (int i = 0; i < guestVector.size(); i++) {
                if (guest_id == i) {     
                    guestMapper->deleteGuest(guestVector[guest_id]);
                   
                    guestVector = guestMapper->getAll();
                    break;
                }
            }
          
            break;
        }
        case 3: {
            std::wstring name;
            std::wstring surname;
            std::wstring patronymic;
            std::wstring gender;
            std::wstring birth_date;
            std::wstring date_e;
            std::wstring date_d;
            int parking_lot;
            std::wstring auto_number;

            int building;
            std::wstring country;
            std::wstring city;
            std::wstring street;

            int passport_number;
            std::wstring issuance;
            std::wstring givenBy;
            int writeId;
            int room_number;
            int addressId;
            int passportId;
            int roomId;
            std::wcout << L"Список всех гостей: " << endl;
            guestVector = guestMapper->getAll();
            for (int i = 0; i < guestVector.size(); i++) {
                std::wcout << L"Индекс " << i << L"; " << L"Имя " << guestVector[i].name << L"; " << L"Фамилия " << guestVector[i].surname << L"; " << L"Отчество "
                    << guestVector[i].patronymic << L"; " << L"Пол " << guestVector[i].gender <<
                    L"; " << L"Дата рождения " << guestVector[i].birthdate <<
                    L"; " << L"Страна " << guestVector[i].address.country << L"; " << L"Город " << guestVector[i].address.city << L"; " << L"Улица "
                    << guestVector[i].address.street
                    << L"; " << L"Дом " << guestVector[i].address.building << L"; " << L"Номер паспорта " << guestVector[i].passport.number << L"; "
                    << L"Дата выдачи паспорта " << guestVector[i].passport.issuance << L"; " << L"Номер комнаты " <<
                    guestVector[i].room.number << L"; " << L"Количество людей в комнате " << guestVector[i].room.people_count << L"; "
                    << L"Номер парк. места " << guestVector[i].parking_lot_number << L"; " << L"Номер авто " <<
                    guestVector[i].auto_number << L"; " << L"Дата приезда " << guestVector[i].date_of_entry << L"; " << L"Дата уезда "
                    << guestVector[i].departure_date << endl;
                wcout << endl;
                wcout << endl;
            }
            std::wcout << L"Введите id гостя для изменения!" << endl;
            while (!(wcin >> writeId)) {
                wcin.clear();
                wcin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::wcout << L"Неверный ввод (введите число)!" << endl;
            }
            for (int i = 0; i < guestVector.size(); i++) {
                if (writeId == i) {
                    for (int i = 0; i < guestVector.size(); i++) {
                        if (writeId == i) {
                            writeId = guestVector[i].id;
                            addressId = guestVector[i].address.id;
                            passportId = guestVector[i].passport.id;
                            roomId = guestVector[i].room.id;
                        }
                    }
                    std::wcout << L"Введите имя" << endl;
                    std::wcin >> name;
                    std::wcout << L"Введите фамилию" << endl;
                    std::wcin >> surname;
                    std::wcout << L"Введите отчество" << endl;
                    std::wcin >> patronymic;
                    std::wcout << L"Введите пол(Женщина, Мужчина)" << endl;
                    std::wcin >> gender;

                    if (strcmp((CHAR*)gender.c_str(), "Мужчина") == 0 || strcmp((CHAR*)gender.c_str(), "Женщина") == 0) {
                        wcout << L"Неверный ввод пола, попробуйте заново. Введите любой символ для продолжения" << endl;
                        std::wstring str;
                        wcin >> str;
                    }

                    std::wcout << L"Введите дату рождения (XX-XX-XXXX)" << endl;
                    std::wcin >> birth_date;
                    bool isValidBD = isValidDate(birth_date);
                    if (isValidBD == 0) {
                        wstring str;
                        wcout << L"Неверно введена дата. Нажмите что нибудь для продолжения" << endl;
                        wcin >> str;
                        break;
                    }
                    std::wcout << L"Введите номер парковочного места" << endl;
                    while (!(wcin >> parking_lot)) {
                        wcin.clear();
                        wcin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::wcout << L"Неверный ввод (введите число)!" << endl;
                    }
                    for (int i = 0; i < guestVector.size(); i++) {
                        if (guestVector[i].parking_lot_number == parking_lot) {
                            wcout << L"Такой номер парковочного места уже существует. Введите что нибудь для продолжения" << endl;
                            wstring str;
                            wcin >> str;
                            guest_menu_cases(connection);
                        }
                    }

                    std::wcout << L"Введите номер автомобиля" << endl;
                    std::wcin >> auto_number;
                    std::wcout << L"Введите дату прибытия" << endl;
                    std::wcin >> date_e;
                    bool isValidDateE = isValidDate(date_e);
                    if (isValidDateE == 0) {
                        wstring str;
                        wcout << L"Неверно введена дата. Нажмите что нибудь для продолжения" << endl;
                        wcin >> str;
                        break;
                    }
                    std::wcout << L"Введите дату уезда" << endl;
                    std::wcin >> date_d;
                    bool isValidDateD = isValidDate(date_d);
                    if (isValidDateD == 0) {
                        wstring str;
                        wcout << L"Неверно введена дата. Нажмите что нибудь для продолжения" << endl;
                        wcin >> str;
                        break;
                    }

                    std::wcout << L"Введите страну" << endl;
                    std::wcin >> country;
                    std::wcout << L"Введите город" << endl;
                    std::wcin >> city;
                    std::wcout << L"Введите улицу" << endl;
                    std::wcin >> street;

                    std::wcout << L"Введите номер дома" << endl;
                    while (!(wcin >> building)) {
                        wcin.clear();
                        wcin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::wcout << L"Неверный ввод (введите число)!" << endl;
                    }

                    std::wcout << L"Введите номер пасспорта" << endl;
                    while (!(wcin >> passport_number)) {
                        wcin.clear();
                        wcin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::wcout << L"Неверный ввод (введите число)!" << endl;
                    }
                    for (int i = 0; i < guestVector.size(); i++) {
                        if (guestVector[i].passport.number == passport_number) {
                            wcout << L"Такой номер паспорта уже существует. Введите что нибудь для продолжения" << endl;
                            wstring str;
                            wcin >> str;
                            guest_menu_cases(connection);
                        }
                    }

                    std::wcout << L"Введите дату выдачи паспорта" << endl;
                    std::wcin >> issuance;
                    bool isValidIssuance = isValidDate(issuance);
                    if (isValidIssuance == 0) {
                        wstring str;
                        wcout << L"Неверно введена дата. Нажмите что нибудь для продолжения" << endl;
                        wcin >> str;
                        break;
                    }
                    std::wcout << L"Введите кем выдан пасспорт" << endl;
                    std::wcin >> givenBy;

                    wcout << L"Введите номер комнаты заселения" << endl;
                    while (!(wcin >> room_number)) {
                        wcin.clear();
                        wcin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::wcout << L"Неверный ввод (введите число)!" << endl;
                    }
                    int flag = -1;

                    while (flag != 1) {
                        for (int i = 0; i < roomVector.size(); i++) {
                            if (room_number == roomVector[i].number) {
                                flag = 1;
                                room_number = roomVector[i].number;
                                RoomMapper roomMapper = RoomMapper(connection);
                                Room room1 = roomMapper.findRoomByNumber(room_number);

                                Address updatedAddress(addressId, country, city, street, building);
                                Passport updatedPassport(passportId, passport_number, issuance, givenBy);

                                Room room = Room(room1.id, room1.number, room1.people_count);
                                Guest updatedGuest = Guest(writeId, name, surname, patronymic, gender, birth_date, updatedAddress, updatedPassport, room1, parking_lot, auto_number, date_e, date_d);
                                guestMapper->updateGuest(updatedGuest);
                                guestVector = guestMapper->getAll();
                                break;
                            }
                            else { flag = 0; }
                        }
                        if (flag == 1) {
                            break;
                        }
                        else {
                            flag = 0;
                            wcout << L"Введите номер комнаты которая существует!" << endl;
                            wcin >> room_number;
                        }

                    }
                    
                    break;
                }
            }
            break;
        }
        case 4: {
            std::wcout << L"Список всех гостей: " << endl;
            guestVector = guestMapper->getAll();
            for (int i = 0; i < guestVector.size(); i++) {
                std::wcout <<L"Индекс " << i << L"; " << L"Имя " << guestVector[i].name << L"; " << L"Фамилия " << guestVector[i].surname << L"; " << L"Отчество "
                    << guestVector[i].patronymic << L"; " << L"Пол " << guestVector[i].gender <<
                     L"; " << L"Дата рождения " << guestVector[i].birthdate<<
                     L"; " << L"Страна " << guestVector[i].address.country << L"; " << L"Город " << guestVector[i].address.city << L"; " << L"Улица "
                    << guestVector[i].address.street
                    << L"; " << L"Дом " << guestVector[i].address.building <<L"; " << L"Номер паспорта " << guestVector[i].passport.number << L"; " 
                    << L"Дата выдачи паспорта " << guestVector[i].passport.issuance << L"; " << L"Номер комнаты " <<
                    guestVector[i].room.number<<L"; " << L"Количество людей в комнате " <<guestVector[i].room.people_count<<L"; "
                    << L"Номер парк. места " <<guestVector[i].parking_lot_number<<L"; " << L"Номер авто " <<
                    guestVector[i].auto_number<<L"; " << L"Дата приезда " <<guestVector[i].date_of_entry<<L"; " << L"Дата уезда " 
                    <<guestVector[i].departure_date<< endl;
                wcout << endl;
                wcout << endl;
            }

            std::wcout << L"Введите любую клавишу для продолжения!" <<std::endl;
            std::wstring wait;
            std::wcin >> wait;
            break;
        }
        case 5: {
            return;

            break;
        }
        case 6: {
            exit(0);

            break;
        }
        }
    }
}

void service_menu_options() {
    setlocale(LC_ALL, "rus_rus.866");
    _setmode(_fileno(stdout), _O_U16TEXT);
    system("cls"); 
    std::wcout << L"Выберите действие!" << endl;
    std::wcout << L"1. Добавить сервис" << endl;
    std::wcout << L"2. Удалить сервис" << endl;
    std::wcout << L"3. Весь список назначенных сервисов" << endl;
    std::wcout << L"4. В главное меню" << endl;
    std::wcout << L"5. Выход" << endl;
    std::wcout << L">";
}

void service_menu_cases(SQLConnection* connection) {
    ServiceMapper serviceMapper = ServiceMapper(connection);
    Mapper guestMapper = Mapper(connection);
    std::vector<Guest> guestVector = guestMapper.getAll();
    ServiceGuestMapper serviceGuestMapper = ServiceGuestMapper(connection);
    std::vector<ServiceGuest> serviceGuestVector = serviceGuestMapper.getAll();
    int choice = 0;
    std::vector<Service> serviceVector = serviceMapper.getAll();

   
    while (choice<5) {
        service_menu_options(); 
       
       
        while (!(wcin >> choice)) {
           
            wcin.clear();
            wcin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::wcout << L"Неверный ввод (введите номер пункта)!" << endl;
            
        }
       
        switch (choice) {
        case 1: {
            int wait;
            std::wstring service_name;
            int guest_id = 0;
            Mapper* guestMapper = new Mapper(connection);
            vector<Guest> guestVector;

            std::wcout << L"Введите название сервиса(Бассейн, Спортзал, Игровая)" << endl;
            std::wcin >> service_name;
   
            std::wcout << L"Список всех гостей: " << endl;
            guestVector = guestMapper->getAll();
            for (int i = 0; i < guestVector.size(); i++) {
                std::wcout << L"Индекс " << i << L"; " << L"Имя " << guestVector[i].name << L"; " << L"Фамилия " << guestVector[i].surname << L"; " << L"Отчество "
                    << guestVector[i].patronymic << L"; " << L"Пол " << guestVector[i].gender <<
                    L"; " << L"Дата рождения " << guestVector[i].birthdate <<
                    L"; " << L"Страна " << guestVector[i].address.country << L"; " << L"Город " << guestVector[i].address.city << L"; " << L"Улица "
                    << guestVector[i].address.street
                    << L"; " << L"Дом " << guestVector[i].address.building << L"; " << L"Номер паспорта " << guestVector[i].passport.number << L"; "
                    << L"Дата выдачи паспорта " << guestVector[i].passport.issuance << L"; " << L"Номер комнаты " <<
                    guestVector[i].room.number << L"; " << L"Количество людей в комнате " << guestVector[i].room.people_count << L"; "
                    << L"Номер парк. места " << guestVector[i].parking_lot_number << L"; " << L"Номер авто " <<
                    guestVector[i].auto_number << L"; " << L"Дата приезда " << guestVector[i].date_of_entry << L"; " << L"Дата уезда "
                    << guestVector[i].departure_date << endl;
                wcout << endl;
                wcout << endl;
            }
            std::wcout << L"Введите id гостя" << endl;
            while (!(wcin >> guest_id)) {
                wcin.clear();
                wcin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::wcout << L"Неверный ввод (введите число)!" << endl;
            }

            guest_id = guestVector[guest_id].id;

            Service service = Service(-1, service_name,guest_id);

            Service serviceToGet = serviceMapper.createService(service,guest_id);

            serviceVector.emplace_back(serviceToGet);
            
            serviceVector = serviceMapper.getAll();
            break;
        }

        case 2: {
            int service_id;
            int wait;
   

            std::wcout << L"Список всех сервисов: " << endl;
            wcout << L"Индекс " << setw(15) << L"название сервиса " << setw(15) << L"id гостя" << endl;
            wcout << setfill(L'-') << setw(15) << L"-" << endl;
            wcout << setfill(L' ');
            for (int i = 0; i < serviceVector.size(); i++) {

                wcout << i << setw(15) << serviceVector[i].service_name << setw(15) << serviceVector[i].guest_id << endl;
            }
            std::wcout << L"Введите id сервиса для удаления!" << endl;
            while (!(wcin >> service_id)) {
                wcin.clear();
                wcin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::wcout << L"Неверный ввод (введите число)!" << endl;
            }
            for (int i = 0; i < serviceVector.size(); i++) {
               
                    if (i == service_id) {

                        serviceMapper.deleteService(serviceVector[i]);
                    }
            }
            serviceVector = serviceMapper.getAll();
            

            
            break;
        }
        case 3: {
            Mapper mapper = Mapper(connection);
            guestVector = mapper.getAll();
            std::vector<int>guestIdVector;
            for (int i = 0; i < guestVector.size(); i++) {
                for (int j = 0; j < serviceVector.size(); j++) {
                    if (guestVector[i].id == serviceVector[j].guest_id) {
                        guestIdVector.emplace_back(i);
                    }
                }
            }
            std::wcout << L"Список всех сервисов: " << endl;
            wcout << L"Индекс " << setw(15) << L"название сервиса "<<setw(15)<<L"id гостя" << endl;
            wcout << setfill(L'-') << setw(15) << L"-" << endl;
            wcout << setfill(L' ');
            for (int i = 0; i < serviceVector.size(); i++) {
                
                wcout << i << setw(15) << serviceVector[i].service_name << setw(15)<<guestIdVector[i]<< endl;
            }

            std::wcout << L"Нажмите любую цифру для продолжения!" << std::endl;

            wstring wait;
            std::wcin >> wait;
            break;
        }
        case 4: {
            return;
            break;
        }
        case 5: {
            exit(0);
            break;
        }
        }

    }
}




int main() {
    setlocale(LC_ALL, "rus_rus.866");

    _setmode(_fileno(stdout), _O_U16TEXT);
    SQLConnection connection = SQLConnection();

    connection.init(
        (SQLCHAR*)L"PostgreSQL35W",
        (SQLCHAR*)L"postgres",
        (SQLCHAR*)L""
    );

    Migrations migrations = Migrations(&connection);
    migrations.runMigrations();

    ServiceMapper serviceMapper = ServiceMapper(&connection);
    int choice = 0;

    while(choice<6) {
        main_menu(); 
        
        while (!(wcin >> choice)) {

            wcin.clear();
            wcin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');


            std::wcout << L"Неверный ввод (введите номер пункта)!" << endl;

        }
       
        switch (choice) {
        case 1: {
            guest_menu_cases(&connection);
            break;
        }

        case 2: {
            room_menu_cases(&connection);
            break;
        }
        case 3: {
            service_menu_cases(&connection);
            break;
        }
        case 4: {
            maid_menu_cases(&connection);
            break;
        }
        case 5: {
            schedule_menu_cases(&connection);
            break;
        }
        case 6: {
            return EXIT_SUCCESS;;
            break;
        }
        }
        
    }
    return EXIT_SUCCESS;
}