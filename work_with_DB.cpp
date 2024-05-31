#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "work_with_DB.h"
#include <iostream>
#define SQLITECPP_COMPILE_DLL
#include "SQLiteCpp/SQLiteCpp.h"
#include "time.h"
//#define SQLITECPP_COMPILE_DLL та #define _CRT_SECURE_NO_WARNINGS потрібні для нормального функціонування програми, без них не компілюється

int AddData(int UserID, int Callories)
{
        //Функція для додавання запису про спожиту їжу до БД.
        char Today_date[20];

        SQLite::Database db("example.db3", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
        //SQL запит, який буде виконуватися базою даних
        SQLite::Statement query(db, "INSERT INTO users (id, Dt, calories) VALUES  (?,?,?)");
        
        time_t mytime = time(NULL);

        struct tm* now = localtime(&mytime);

        strftime(Today_date, sizeof(Today_date), "%Y/%m/%d", now);
        //UserID - індивідуальний код для кожного користувача, вийдається телеграмом.
        //Callories - кількість калорій, яку спожив користувач.
        //Today_date - сьогоднішня дата в формі рядку (масив чарів). Дана БД не підтримує Дату, тому потрібно записувати її в формі рядка.
        query.bind(1, UserID);
        query.bind(2, Today_date);
        query.bind(3, Callories);
        query.exec();

    return 0;
}

int GetAverageDataForMonth(int UserID)
{
    return GetDataForMonth(UserID) / 30;
}

int GetDataForMonth(int UserID)
{
    //отримуємо дані про суму спожитих калорій за місяць
  try {
            int totalCalories = 0;
            SQLite::Database db("example.db3", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
            for (int i = 0; i < 30; i++) {

                char Today_date[20];
   
                time_t mytime = time(NULL);

                struct tm* now = localtime(&mytime);
         
                now->tm_mday -= i;

                mktime(now);

                strftime(Today_date, sizeof(Today_date), "%Y/%m/%d", now);
                //SQL запит, який буде виконуватися базою даних
                SQLite::Statement query(db, "SELECT sum(calories) FROM users WHERE id = :userID AND Dt = :userDate");
                //UserID - індивідуальний код для кожного користувача, вийдається телеграмом.
                //Today_date - сьогоднішня дата в формі рядку (масив чарів). Дана БД не підтримує Дату, тому потрібно записувати її в формі рядка.
                query.bind(":userID", UserID);
                query.bind(":userDate", Today_date);
    
                if (query.executeStep())
                {
                    totalCalories += query.getColumn(0).getInt();
        
                }
            }
            return totalCalories;
   }
    catch (std::exception& e) 
    {
        std::cerr << e.what() << std::endl;
    }

}

int GetAverageDataForAllTime(int UserID)
{
    //Різниця між сьогоднішним днем, та днем створення боту.
    int DiferenceBetweenDate = 0;

    time_t mytime = time(NULL);

    struct tm* now = localtime(&mytime);

    int today_day = now->tm_mday;
    //До today_month додається 1 через специфічний спосіб запису даних в комп'ютері
    int today_month = now->tm_mon + 1;
    int today_year = now->tm_year;

    time_t mytime1 = time(NULL);

    struct tm* now1 = localtime(&mytime1);
    //Дата створення боту
    now1->tm_mday = 22;
    now1->tm_mon = 5;
    now1->tm_year = 124;

    mktime(now1);

    DiferenceBetweenDate = (today_day - now1->tm_mday) + (today_month - now1->tm_mon) * 30 + (today_year - now1->tm_year) * 365;

    return GetDataForAllTime(UserID) / DiferenceBetweenDate;
}

int GetDataForAllTime(int UserID)
{
    //Метод дозволяє отримати дані про суму спожитих калорій за весь час
    try {
        int totalCalories = 0;
        SQLite::Database db("example.db3", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
        

            SQLite::Statement query(db, "SELECT sum(calories) FROM users WHERE id = :userID");
            query.bind(":userID", UserID);

            if (query.executeStep())
            {
                totalCalories += query.getColumn(0).getInt();

            }
        
        return totalCalories;
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

}

int GetDataForToday(int UserID)
{
    //Отримати дані про спожиті сьогодні калорії.=
    try {
        int totalCalories = 0;
        SQLite::Database db("example.db3", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);

        char Today_Date[20];

        time_t mytime = time(NULL);

        struct tm* now = localtime(&mytime);

        strftime(Today_Date, sizeof(Today_Date), "%Y/%m/%d", now);
        //SQL запит, який буде виконуватися базою даних
        SQLite::Statement query(db, "SELECT sum(calories) FROM users WHERE id = :userID AND Dt = :userDate");
        //UserID - індивідуальний код для кожного користувача, вийдається телеграмом.
        query.bind(":userID", UserID);
        query.bind(":userDate", Today_Date);

        if (query.executeStep())
        {
            totalCalories += query.getColumn(0).getInt();

        }

        return totalCalories;
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

}
