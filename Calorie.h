#pragma once
#include <string>
using namespace std;

double TotalCal(double gramm, int call);

string PrepareString(string full_str, string search_str);

string PrepareCalorieString(string full_str, string search_str);

string GetName(string full_str, string search_str);

int GetCalorie(string str);

static size_t getResponsetoString(void* contents, size_t size, size_t nmemb, void* userp);

int CaloriesFunc(string Search);

string NameFunc(string Search);

