#ifndef NUM_HPP
#define NUM_HPP
#include <iostream>
#include <type_traits>

struct Num
{
    int x;
    Num(int x = 0) : x(x) {}
    Num(const Num& oth) : x(oth.x) { asm volatile ("nop"); }
    Num& operator=(const Num& oth) {x = oth.x; asm volatile ("nop"); return *this; }
    Num(Num&& oth) : x(oth.x), veryImpotantData(oth.veryImpotantData) { oth.veryImpotantData = nullptr;}
    Num& operator=(Num&& oth) { x = oth.x; std::swap(veryImpotantData, oth.veryImpotantData); return *this;}
    ~Num() {delete veryImpotantData;}
    int* veryImpotantData = new int;
};

std::ostream& operator<<(std::ostream& out, Num t);

Num operator+(const Num& a, const Num& b);
Num operator-(const Num& a, const Num& b);
Num operator*(const Num& a, const Num& b);
Num operator/(const Num& a, const Num& b);
bool operator==(const Num& a, const Num& b);
bool operator< (const Num& a, const Num& b);
Num operator-(const Num& x);

#endif /* NUM_HPP */
