#include <iostream>
#pragma once

struct Char
{
    int a;
    Char(int a) : a (a) {}
};

int operator+(const Char& a, const Char& b)
{
    return a.a * b.a;
}

std::ostream& operator<<(std::ostream& os, const Char& a)
{
    return os<<a.a;
}

#define char Char
