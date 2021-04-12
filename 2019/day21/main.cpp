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

// #include <boost/graph/adjacency_list.hpp>
// #include <boost/graph/dijkstra_shortest_paths.hpp>
// #include <boost/property_map/property_map.hpp>

std::vector<std::string> readMaze(std::string filename)
{
    std::vector<std::string> raw_maze;

    std::ifstream input("input.txt");
    input.imbue(std::locale(input.getloc(), new Advent::ctype));

    auto input_iter = std::istream_iterator<std::string>(input);
    while(input_iter != std::istream_iterator<std::string>())
    {
        raw_maze.push_back(*input_iter);
        ++input_iter;
    }

    return raw_maze;
}

using Point = std::pair<int, int>;

enum class Direction : std::int64_t
{
    North,
    South,
    West,
    East,
};

Point add (Point a, Point b)
{
    return { a.first + b.first, a.second + b.second };
}

Point delta (Direction value)
{
    switch(value)
    {
        case Direction::North:
            return { 0, -1 };
        case Direction::South:
            return { 0, 1 };
        case Direction::West:
            return { -1, 0 };
        case Direction::East:
            return { 1, 0 };
        default:
            throw std::runtime_error("Invalid value");
    }
}

std::array<Point, 4> cardinalPoints(Point p)
{
    return { add(p, delta(Direction::North)),
             add(p, delta(Direction::South)),
             add(p, delta(Direction::East)),
             add(p, delta(Direction::West)) };
}

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

    std::istream& input;
    std::ostream& output;
};


int main(int argc, char* argv[])
{
    auto memory = Intcode::parseFile("input.txt");

    ASCIIQueue queue;    

    Intcode::Context ctx { memory, queue, queue };

    Intcode::run(ctx);

    // Part 1
    // NOT C J
    // NOT A T
    // OR T J
    // AND D J
    // RUN

    // Part 2

    return 0;
}
