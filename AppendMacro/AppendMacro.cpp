// AppendMacro.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>

#define p(c) printf(c);

#define HOOKALL p("1");

#define APPENDED HOOKALL p("2");
#undef HOOKALL
// #define HOOKALL APPENDED;


int main()
{
    APPENDED;
    std::cout << "\ndone!\n"; 
}

