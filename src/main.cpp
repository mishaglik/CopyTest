#include "Logger.hpp"
#include "Watcher.hpp"
#include "Num.hpp"
#include <cstddef>
#include <ostream>
#include <vector>
#include <cmath>

Logger LOG("img.png");

using WNum = Watcher<Num>;

const WNum INF_SOL(-1, "INF_SOL");

WNum sqrt(const WNum&)
{
    //Super duper sqrt calcluator;
    return WNum(2);
}

struct Solution
{
    WNum nRoots = "nRoots";
    WNum roots[2] = {{"roots[0]"}, {"roots[1]"}};
    Solution() {}
    Solution(const Solution& sol)
        : nRoots(sol.nRoots), roots{sol.roots[0], sol.roots[1]} {}
    Solution(Solution &&sol)
        : nRoots(sol.nRoots), roots{sol.roots[0], sol.roots[1]} {}
    Solution& operator=(const Solution& sol) {
      nRoots = sol.nRoots;
      roots[0] = sol.roots[0];
      roots[1] = sol.roots[1];
      return *this;
    }
    Solution& operator=(Solution&& sol) {
      nRoots = sol.nRoots;
      roots[0] = sol.roots[0];
      roots[1] = sol.roots[1];
      return *this;
    }
};

using WSolution = Watcher<Solution>;

WSolution solveLinEq(const WNum& a, const WNum& b)
{
    FUNC_INSPECT
    WSolution solution("solution");

    if(a == WNum(0))
    {
        if(b == WNum(0))
        {
            solution.nRoots = INF_SOL;
            return solution;
        }
        else {
            solution.nRoots = WNum(0);
            return solution; 
        }
    }
    solution.nRoots = WNum(1);
    solution.roots[0] = -b / a;
    return solution;
}

WSolution solveSqEq(const WNum& a, const WNum& b, const WNum& c)
{
    FUNC_INSPECT
    WSolution solution("solution");

    if(a == WNum(0))
    {
        solution = solveLinEq(b, c);
        return solution;
    }
    WNum d2 = b * b - WNum(4) * a * c;
    if(d2 < WNum(0))
    {
        solution.nRoots = WNum(0);
        return solution;
    }
    else if( d2 == WNum(0))
    {
        solution.nRoots = WNum(1);
        return solution;
    }
    solution.nRoots = WNum(2);
    solution.roots[0] = (-b + sqrt(d2)) / (WNum(2) * a);
    solution.roots[1] = (-b - sqrt(d2)) / (WNum(2) * a);
    return solution;
}

void printSolution(const Solution& sol)
{
    std::cout << sol.nRoots.x << '\n';
}

int main()
{
    FUNC_INSPECT
    WNum b(20, "b");
    WNum c(2, "c");
    // WSolution solution = solveSqEq(WNum(1), b, c);
    WSolution solution = solveLinEq(WNum(1), b);
    printSolution(solution);
    return 0;
}