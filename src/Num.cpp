#include "Num.hpp"

std::ostream& operator<<(std::ostream& out, Num t)
{
    return out << t.x;
}

Num operator+(Num a, Num b)
{
    return Num(a.x + b.x);
}

Num operator-(Num a, Num b)
{
    return Num(a.x - b.x);
}

Num operator*(Num a, Num b)
{
    return Num(a.x * b.x);
}

Num operator/(Num a, Num b)
{
    return Num(a.x / b.x);
}

bool operator==(Num a, Num b)
{
    return a.x == b.x;
}

bool operator<(Num a, Num b)
{
    return a.x < b.x;
}

Num operator-(Num x)
{
    return Num(-x.x);
}
