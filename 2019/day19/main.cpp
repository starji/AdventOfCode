#include <iostream>
#include <locale>
#include <map>
#include <iterator>
#include <cctype>
#include <algorithm>
#include <cstdlib>
#include <set>
#include <cmath>
#include <cstdint>
#include <limits>
#include <stack>
#include <fstream>
#include <numeric>
#include <cstdio>

#include "Advent.hpp"
#include "Intcode.hpp"


using Point = std::pair<int, int>;
using Field = std::map<Point, std::int64_t>;

struct PointQueue : public Intcode::Queue
{
    PointQueue(Point point)
        : point(point)
    {}

    void push(std::int64_t value) override
    {
        output = value;
    }

    std::int64_t pop() override
    {
        if(x_coord)
        {
            x_coord = false;
            return point.first;
        }
        else
        {
            return point.second;
        }
    }

    Point point;
    std::int64_t output;
    bool x_coord = true;
};

std::int64_t getValue(Point point)
{
    static const Intcode::Memory memory = Intcode::parseFile("input.txt");
    PointQueue queue (point);
    Intcode::Context ctx { memory, queue, queue };
    Intcode::run(ctx);
    return queue.output;
}

bool testSquare(Point point, int side)
{
    auto side_pt = side - 1;
    return getValue({point.first, point.second}) == 1 &&
        getValue({point.first + side_pt, point.second}) == 1 &&
        getValue({point.first, point.second + side_pt}) == 1 &&
        getValue({point.first + side_pt, point.second + side_pt}) == 1;
}

Point findY(int x)
{
    int y = 0;
    while(getValue({x, y}) == 0)
    {
        ++y;
    }
    return { x, y };
}

void testRegion(Point p, int side)
{
    std::cout << "Testing " << p.first << ',' << p.second << "=" << (testSquare(p, side) ? "Valid" : "Invalid")
              << std::endl;
}

int checkWidth(int y)
{
    int count = 0;

    auto init_x = ((35*y)/100) - 2;
    for(auto x = init_x; x < init_x + 139; ++x)
    {
        Point p {x, y};
        if(getValue(p) == 1)
        {
            ++count;
        }
    }
    return count;
}

int main(int argc, char* argv[])
{
    // Field field;

    // for(auto y = 760; y < 820; ++y)
    // {
    //     for(auto x = 285; x < 385; ++x)
    //     {
    //         Point p {x, y};
    //         field[p] = getValue(p);
    //     }
    // }

    // Advent::printMap(field.begin(), field.end(),
    //                  [](auto p)
    //                  {
    //                      switch(p)
    //                      {
    //                          case 0:
    //                              return '.';
    //                          case 1:
    //                              return '#';
    //                          default:
    //                              throw std::runtime_error("unknown thingo");
    //                      }
    //                  });

    // auto y = 300;
    // while(checkWidth(y) < 135)
    // {
    //     ++y;
    // }

    // std::cout << "Y at first 135 = " << y << std::endl;

    // auto p = findY(165);
    // std::cout << p.first << "," << p.second << std::endl;

    // 165,339

    std::map<Point, bool> good;

    for(auto y = 760; y < 1000; ++y)
    {
        for(auto x = 285; x < 400; ++x)
        {
            std::cout << "testing: " << x << ',' << y << std::endl;
            good[{x, y}] = testSquare({x, y}, 100);
        }
    }

    for(auto& p: good)
    {
        if(p.second)
        {
            std::cout << "Good: " << p.first.first << ',' << p.first.second << std::endl;
        }
    }

    // testRegion({191,340}, 100);
    // testRegion({191,341}, 100);

    // std::cout << "Affected=" << std::count_if(field.begin(), field.end(),
    //                                           [](auto p)
    //                                           {
    //                                               return p.second == 1;
    //                                           })
    //           << std::endl;

    return 0;
}
