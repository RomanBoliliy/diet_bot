#pragma once
#include "Calculate.h"

//Функції для розрахунку калорій для людини. Всі формули взяті з інтернету
int NeedCalorie(int age, int weight, int height, bool isMan, int activity)
{
    int res = 0; 
    float coeficient = 0;
    switch (activity)
    {
    case 1: coeficient = 1.2; break;
    case 2: coeficient = 1.375; break;
    case 3: coeficient = 1.55; break;
    case 4: coeficient = 1.725; break;
    case 5: coeficient = 1.9; break;
    default:
        coeficient = 1.55;
        break;
    }

    if (isMan) {
        res = coeficient * (weight * 10) + (height * 6.25) - (age * 5) + 5;
    }
    else {
        res = coeficient * (weight * 10) + (height * 6.25) - (age * 5) - 161;
    }

    return res;
}

int NeedCalorieToLose(int age, int weight, int height, bool isMan, int activity) {
    return NeedCalorie(age, weight, height, isMan, activity) - 500;
}

int NeedCalorieToGain(int age, int weight, int height, bool isMan, int activity) {
    return NeedCalorie(age, weight, height, isMan, activity) + 500;
}

