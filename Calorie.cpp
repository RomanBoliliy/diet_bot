#pragma once
#define CURL_STATICLIB
#include <iostream>
#include <curl/curl.h>
#include <string>
#include "Calorie.h"
//#define CURL_STATICLIB потрібен для нормального функціонування програми

string PrepareString(string full_str, string search_str)
{
    //функція для відкидання непотрібних даних з HTML, який ми отримуємо з сайту.
    string Cuted_str, Transitional_str, Result_str;
    //відкидаємо все, що вище та нижче ключових слів (Хедер та Футер)
    size_t Cuted_str_preparing = full_str.find("Алкогольні напої");
    size_t Cuted_str_end = full_str.find("aggregateRating");
    Cuted_str = full_str.substr(Cuted_str_preparing, (Cuted_str_end - Cuted_str_preparing));

    size_t Cuted_str_start = Cuted_str.find("ккал");
    Cuted_str = Cuted_str.substr(Cuted_str_start + 5);
    //Відкидаємо всі заголовки, які зустрічаються в нашому рядку (вони заважають нормальному пошуку, та приводять до помилок)
    size_t BezAlkohol = Cuted_str.find("Безалкогольні");
    Cuted_str.erase(BezAlkohol, 19);

    size_t Kashi = Cuted_str.find("Каші");
    Cuted_str.erase(Kashi, 4);

    size_t Gribi = Cuted_str.find("Гриби");
    Cuted_str.erase(Gribi, 5);

    size_t Ikra = Cuted_str.find("Ікра");
    Cuted_str.erase(Ikra, 4);

    size_t Maslo = Cuted_str.find("Масло");
    Cuted_str.erase(Maslo, 21);

    size_t Molochni = Cuted_str.find("Молочні");
    Cuted_str.erase(Molochni, 16);

    size_t Yaitsya = Cuted_str.find("Яйця");
    Cuted_str.erase(Yaitsya, 4);

    size_t Kovbasa = Cuted_str.find("Ковбаса");
    Cuted_str.erase(Kovbasa, 26);

    size_t Ptah = Cuted_str.find("птах");
    Cuted_str.erase(Ptah - 18, 22);

    size_t Riba = Cuted_str.find("Риба");
    Cuted_str.erase(Riba, 19);

    size_t Ovochi = Cuted_str.find("Овочі");
    Cuted_str.erase(Ovochi, 5);

    size_t Frukti = Cuted_str.find("Фрукти");
    Cuted_str.erase(Frukti, 14);

    size_t Gorihi = Cuted_str.find("Горіхи");
    Cuted_str.erase(Gorihi, 19);

    size_t Solodoshchi = Cuted_str.find("Солодощі");
    Cuted_str.erase(Solodoshchi, 8);

    size_t Hlib = Cuted_str.find("Хліб");
    Cuted_str.erase(Hlib, 26);

    //Записуємо Prepare.find(search_str) - 20, бо користувач може вводити назву продукту не з початку, таким чином він може почати з середини, але програма все одно знайде продукт
    size_t Transitional_str_start = Cuted_str.find(search_str) - 20;
    //Res1_start == string::npos означає, що не вдалося знайти такий продукт.
    if (Transitional_str_start == string::npos)
    {
        return "0";
    }
    else {

        Transitional_str = Cuted_str.substr(Transitional_str_start);

        size_t Result_str_start = Transitional_str.find(search_str) - 20;
        size_t Result_str_try = Transitional_str.find("</tr");
        size_t Result_str_end;
        //Обчислення, зв'язані з Prepare.find(search_str) - 20;
        if (Result_str_try < 20)
        {
            Result_str_end = Transitional_str.find("</tr", Result_str_try + 15);
        }
        else {
            Result_str_end = Transitional_str.find("</tr");
        }

        Result_str = Transitional_str.substr(Result_str_start, Result_str_end - Result_str_start);

        return Result_str;

    }
}

string PrepareCalorieString(string full_str, string search_str)
{
    string Res, Res2;
    Res = PrepareString(full_str, search_str);
    if (Res == "0") { return "0"; }
    else {
        size_t Res_start2 = Res.find("<td>");
        Res2 = Res.substr(Res_start2);
        return Res2;
    }
}

string GetName(string full_str, string search_str)
{
    //Функція для отримання назви страви
    string Prepared_str, NameofDish, Prepared_substring;
    Prepared_str = PrepareString(full_str, search_str);
    //Якщо не знайдено, повертаємо нуль, інакше повертаємо назву.
    if (Prepared_str == "0") { return "0"; }
    else {
        size_t Prepared_substring_start = Prepared_str.find("<td>");
        Prepared_substring = Prepared_str.substr(Prepared_substring_start);
        Prepared_substring.erase(0, 4);
        size_t NameofDish_end = Prepared_substring.find("</td>");
        NameofDish = Prepared_substring.substr(0, NameofDish_end);
        return NameofDish;
    }

    
}

int GetCalorie(string Prepared_str)
{
    //Функція для отримання кількості калорій в певному продукті, як натуральне число
    if (Prepared_str == "0")
    {
        return 0;
    }
    string Result_str;

    Prepared_str.erase(Prepared_str.end()-6, Prepared_str.end());

    size_t Callory = Prepared_str.find_last_of("<tr>");
    Result_str = Prepared_str.substr(Callory);
    Result_str.erase(0, 1);

    int result = atoi(Result_str.c_str());
    return result;
}

size_t getResponsetoString(void* contents, size_t size, size_t nmemb, void* userp)
{
    //Функція для отримання даних з сайту в формі рядку (string)
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

int CaloriesFunc(string Search)
{
    //Функція для підключення до сайту та отримання даних про калорії
    string str_response;
    CURL* curl;
    CURLcode response;

    curl = curl_easy_init();

    curl_easy_setopt(curl, CURLOPT_URL, "https://massage.co.ua/uk/tablica-kalorijnosti-produktov-v-100-grammax/");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, getResponsetoString);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &str_response);

    response = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    
    return GetCalorie(PrepareCalorieString(str_response, Search));
}

string NameFunc(string Search)
{
    //Функція для підключення до сайту та отримання даних про назву продукту
    string str_response;
    CURL* curl;
    CURLcode response;

    curl = curl_easy_init();

    curl_easy_setopt(curl, CURLOPT_URL, "https://massage.co.ua/uk/tablica-kalorijnosti-produktov-v-100-grammax/");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, getResponsetoString);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &str_response);

    response = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    return GetName(str_response, Search);
}

double TotalCal(double gramm, int call)
{
    //Функція для отримання інформації про загальну кількість спожитих калорій з однієї страви
    return call * gramm / 100;
}