#pragma once
#include <stdio.h>
#include <tgbot/tgbot.h>
#include "Calorie.h"
#include "Calculate.h"
#include "work_with_DB.h"
#include <vector>
#include <string>
using namespace TgBot;

//Вектор команд, для комфортного оброблення повідомлень, які отримує бот.
vector <string> bot_commands = { "start", "adddish", "caloriestoday","caloriesmonth","caloriesmonth","caloriesalltime", "needcalories","forlose","forgain", };
//Змінні, які використовуються для обчислення кількості калорій, необхідних для людей.
int age, weight, height, activity;
bool isMan;


int main() {
   
    Bot bot("ТУТ ПОВИНЕН БУТИ ТОКЕН");
    
    //Далі описані обробники для всіх команд, які були додані в боті.

    bot.getEvents().onCommand("start", [&bot](Message::Ptr message) {
        bot.getApi().sendMessage(message->chat->id, "Hi!");
    });

    bot.getEvents().onCommand("adddish", [&bot](Message::Ptr message) {
        
        bot.getApi().sendMessage(message->chat->id, "Enter name of product, than size: ");

        bot.getEvents().onAnyMessage([&bot](Message::Ptr message) {

            printf("User wrote %s\n", message->text.c_str());

            for (const auto& command : bot_commands)
            {
                if ("/" + command == message->text)
                {
                    return;
                }

            }

            string UserMessage = message->text.c_str();
            if (UserMessage.length() > 3) {

                size_t str_end = UserMessage.find(",");
                if (str_end == string::npos)
                {
                    return;
                }

                int total = 0;

                string str1 = UserMessage.substr(0, str_end);
                UserMessage.erase(0, str_end + 1);
                int size = stoi(UserMessage);

                string NameOfProduct = NameFunc(str1);
                string Calories = to_string(CaloriesFunc(str1));
                int CaloriesInt = CaloriesFunc(str1);
                total += TotalCal(size, CaloriesInt);
                //Якщо не знайдено страви (CaloriesInt == 0), виводимо повідомлення про це.
                if (CaloriesInt == 0) {
                    bot.getApi().sendMessage(message->chat->id, "Error message, try again");
                    return;
                }
                else {

                    AddData(message->chat->id, total);
                    bot.getApi().sendMessage(message->chat->id, "You pick: " + NameOfProduct + ". It have calories per 100 gramm: " + Calories + ". It have total calories : " + to_string(total));
                    return;
                }
            }
            else {

                bot.getApi().sendMessage(message->chat->id, "Too short message, try again");
                return;
            }
            return;
        });

    });

    bot.getEvents().onCommand("caloriestoday", [&bot](Message::Ptr message) {
     

        bot.getApi().sendMessage(message->chat->id, "Today you eat: " + to_string(GetDataForToday(message->chat->id)) + " calories.");

    });

    bot.getEvents().onCommand("caloriesmonth", [&bot](Message::Ptr message) {
       
        
        bot.getApi().sendMessage(message->chat->id, "Average callories for month is: " + to_string(GetAverageDataForMonth(message->chat->id)) + " calories.");

    });

    bot.getEvents().onCommand("caloriesalltime", [&bot](Message::Ptr message) {
     

        bot.getApi().sendMessage(message->chat->id, "Average callories for all time is: " + to_string(GetAverageDataForAllTime(message->chat->id)) + " calories.");

    });

    bot.getEvents().onCommand("needcalories", [&bot](Message::Ptr message) {
        
        //Нажаль не знайшов способу отримувати повідомлення по черзі, тому всі дані користувач повинен вводити в одному повідомленні, через кому.
        bot.getApi().sendMessage(message->chat->id, "Enter age, weight, height, gender(1 if man, 2 if woman), activity (1-5)");
        //Обробка отриманих даних
        bot.getEvents().onAnyMessage([&bot](Message::Ptr message) {
            string str;
            vector<int> vec;
            str = message->text.c_str();
            for (int i = 0; i < 5; i++) {

                size_t str_end = str.find(",");
                string str1 = str.substr(0, str_end);
                str.erase(0, str_end + 1);
                vec.push_back(stoi(str1));
            }
            int age = vec[0], weight = vec[1], height = vec[2], activity = vec[4];
            bool isMan = false;
            if (vec[3] == 1) { isMan = true; }
            
            bot.getApi().sendMessage(message->chat->id, "You need " + to_string(NeedCalorie(age, weight, height, isMan, activity)) + " calories.");
        });

    });

    bot.getEvents().onCommand("forlose", [&bot](Message::Ptr message) {
        //Така ж ситуація як і з попередньою командою
        bot.getApi().sendMessage(message->chat->id, "Enter age, weight, height, gender(1 if man, 2 if woman), activity (1-5)");

        bot.getEvents().onAnyMessage([&bot](Message::Ptr message) {
            string str;
            vector<int> vec;
            str = message->text.c_str();
            for (int i = 0; i < 5; i++) {

                size_t str_end = str.find(",");
                string str1 = str.substr(0, str_end);
                str.erase(0, str_end + 1);
                vec.push_back(stoi(str1));
            }
            int age = vec[0], weight = vec[1], height = vec[2], activity = vec[4];
            bool isMan = false;
            if (vec[3] == 1) { isMan = true; }

            bot.getApi().sendMessage(message->chat->id, "You need " + to_string(NeedCalorieToLose(age, weight, height, isMan, activity)) + " calories.");
        });

    });

    bot.getEvents().onCommand("forgain", [&bot](Message::Ptr message) {
        //Така ж ситуація як і з попередньою командою
        bot.getApi().sendMessage(message->chat->id, "Enter age, weight, height, gender(1 if man, 2 if woman), activity (1-5)");

        bot.getEvents().onAnyMessage([&bot](Message::Ptr message) {
            string str;
            vector<int> vec;
            str = message->text.c_str();
            for (int i = 0; i < 5; i++) {

                size_t str_end = str.find(",");
                string str1 = str.substr(0, str_end);
                str.erase(0, str_end + 1);
                vec.push_back(stoi(str1));
            }
            int age = vec[0], weight = vec[1], height = vec[2], activity = vec[4];
            bool isMan = false;
            if (vec[3] == 1) { isMan = true; }

            bot.getApi().sendMessage(message->chat->id, "You need " + to_string(NeedCalorieToGain(age, weight, height, isMan, activity)) + " calories.");
        });

    });
    
    bot.getEvents().onAnyMessage([&bot](Message::Ptr message) {
        //Якщо користувач щось пише без команди, це повідомлення виводиться розробнику в консоль. На стороні користувача нічого відбуватися не буде.
        printf("User wrote %s\n", message->text.c_str());
        return;
       });

    //Частина коду, спільна для будь якого боту
    try {
        printf("Bot username: %s\n", bot.getApi().getMe()->username.c_str());
        TgLongPoll longPoll(bot);
        while (true) {
            printf("Long poll started\n");
            longPoll.start();
        }
    }
    catch (TgException& e) {
        printf("error: %s\n", e.what());
    }
    return 0;
}