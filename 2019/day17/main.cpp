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
using Field = std::map<Point, char>;

struct ASCIIQueue : public Intcode::Queue
{
    ASCIIQueue()
        : input(std::cin), output(std::cout)
    {}

    ASCIIQueue(std::istream& input, std::ostream& output)
        : input(input), output(output)
    {}

    void push(std::int64_t value) override
    {
        auto c = static_cast<char>(value);
        
        // if(value == 10)
        // {
        //     current = std::make_pair(0, current.second + 1);
        // }
//        else if (value > 256)
        if(value > 250)
        {
            std::cout << "|Dust Collected: " << value << "|" << std::endl;
        }
        // else
        // {
        //     field[current] = c;
        //     current.first++;
        // }

        std::cout << c;
    }

    std::int64_t pop() override
    {
        return std::getchar();
    }

    Point current { 0, 0 };
    Field field;

    std::istream& input;
    std::ostream& output;
};


int main(int argc, char* argv[])
{
    auto& input = std::cin;
    input.imbue(std::locale(input.getloc(), new Advent::ctype));

    ASCIIQueue queue;

    auto memory = Intcode::parseFile("shawn.txt");
    memory[0] = 2;
    Intcode::Context ctx { memory, queue, queue };
    Intcode::run(ctx);

    // auto& field = queue.field;
    // std::vector<std::pair<Point, char> > intersections;
    // std::copy_if(field.begin(), field.end(), std::back_inserter(intersections),
    //              [&](auto p)
    //              {
    //                  auto above = std::make_pair(p.first.first, p.first.second - 1);
    //                  auto below = std::make_pair(p.first.first, p.first.second + 1);
    //                  auto right = std::make_pair(p.first.first + 1, p.first.second);
    //                  auto left = std::make_pair(p.first.first - 1, p.first.second);
    //                  return p.second == '#' &&
    //                      field[below] == '#' &&
    //                      field[right] == '#' &&
    //                      field[above] == '#' &&
    //                      field[left] == '#';
    //              });

    // int result = 0;
    // for(auto& p : intersections)
    // {
    //     result += p.first.first * p.first.second;
    // }

    // std::cout << result << std::endl;

    return 0;
}
