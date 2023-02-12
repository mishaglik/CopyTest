#include "Num.hpp"

std::ostream& operator<<(std::ostream& out, Num t)
{
    return out << t.x;
}

Num operator+(const Num& a, const Num& b)
{
    return Num(a.x + b.x);
}

Num operator-(const Num& a, const Num& b)
{
    return Num(a.x - b.x);
}

Num operator*(const Num& a, const Num& b)
{
    return Num(a.x * b.x);
}

Num operator/(const Num& a, const Num& b)
{
    return Num(a.x / b.x);
}

bool operator==(const Num& a, const Num& b)
{
    return a.x == b.x;
}

bool operator<(const Num& a, const Num& b)
{
    return a.x < b.x;
}

Num operator-(const Num& x)
{
    return Num(-x.x);
}
