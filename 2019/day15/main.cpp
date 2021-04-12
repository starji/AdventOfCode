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

#include "Advent.hpp"
#include "Intcode.hpp"


using Point = std::pair<int, int>;

Point add (Point a, Point b)
{
    return { a.first + b.first, a.second + b.second };
}

// using Graph = boost::undirected_graph<boost::no_property>;
// using VertexId = boost::graph_traits<Graph>::vertex_descriptor;

enum class Direction : std::int64_t
{
    North = 1,
    South = 2,
    West = 3,
    East = 4,
};

Direction opposite(Direction d)
{
    switch(d)
    {
        case Direction::North:
            return Direction::South;
        case Direction::South:
            return Direction::North;
        case Direction::West:
            return Direction::East;
        case Direction::East:
            return Direction::West;
        default:
            throw std::runtime_error("Invalid direction in opposite");
    }
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

struct MapValue
{
    enum class Value
    {
        Empty,
        Origin,
        Robot,
        Wall,
        Oxygen
    };

    MapValue()
        : value(MapValue::Value::Wall), unexplored({})
    {}

    MapValue(Value v)
        : value(v), unexplored({Direction::North, Direction::South, Direction::West, Direction::East})
    {}

    char printable()
    {
        switch(value)
        {
            case Value::Origin:
                return 'O';
            case Value::Wall:
                return '#';
            case Value::Empty:
                return '.';
            case Value::Robot:
                return '*';
            case Value::Oxygen:
                return 'X';
            default:
                return ' ';
        }
    }

    Value value;
    std::set<Direction> unexplored;
};

struct Robot : Intcode::Queue
{
    enum class State
    {
        Idle,
        WaitingForOutput,
    };

    Robot()
    {
        field[current] = MapValue(MapValue::Value::Origin);
        backtrace.push(Direction::South);
    }

    std::int64_t pop() override
    {
        if(backtrace.empty())
        {
            throw std::runtime_error("Done");
        }

        Direction value;
        // std::cout << "Dir (1N,2S,3W,4E): ";
        // std::cin >> value;

        if(field[current].unexplored.empty())
        {
            value = backtrace.top();
            backtrace.pop();
        }
        else
        {
            auto iter = field[current].unexplored.begin();
            value = *iter;
            field[current].unexplored.erase(iter);
            backtrace.push(opposite(value));
        }

        last_move = value;
        return static_cast<std::int64_t>(value);
    }

    void push(std::int64_t value) override
    {
        auto target = add(current, delta(last_move));
        switch(value)
        {
            case 0:
            {
                // Wall
                field[target] = MapValue {MapValue::Value::Wall};
                // Didn't move
                backtrace.pop();
            }
            break;
                
            case 1:
            {
                auto iter = field.find(target);
                if(iter == field.end())
                {
                    field[target] = MapValue { MapValue::Value::Empty };
                    field[target].unexplored.erase(backtrace.top());
                }
                current = target;
            }
            break;
                
            case 2:
            {
                auto iter = field.find(target);
                if(iter == field.end())
                {
                    field[target] = MapValue { MapValue::Value::Oxygen };
                    field[target].unexplored.erase(backtrace.top());
                }
                current = target;
                test_length = backtrace.size() - 1;
            }
            break;
                
            default:
                throw std::runtime_error("Invalid status");
        }
    }

    void print()
    {
        Advent::printMap(field.begin(), field.end(),
                         [](auto s)
                         {
                             return s.printable();
                         });
    }

    std::map<Point, MapValue> field;
    Point current { 0, 0 };

    std::stack<Direction> backtrace;
    Direction last_move;

    int test_length = 0;
};

int main(int argc, char* argv[])
{
    auto& input = std::cin;
    input.imbue(std::locale(input.getloc(), new Advent::ctype));

    Robot robot;

    Intcode::Context ctx { Intcode::parseFile(argv[1]), robot, robot };

    try
    {
        Intcode::run(ctx);
    }
    catch(...) {}
    robot.print();
    std::cout << "Test: " << robot.test_length << std::endl;

    auto minute = 0;
    auto& field = robot.field;
    auto any_empty = [&] (auto& f)
                     {
                         return std::any_of(f.begin(), f.end(),
                                            [](auto& mv)
                                            {
                                                return mv.second.value == MapValue::Value::Empty;
                                            });
                     };

    auto other_points =  [&] (auto& point) -> std::array<Point, 4>
        {
            return { add(point, delta(Direction::North)),
                     add(point, delta(Direction::South)),
                     add(point, delta(Direction::East)),
                     add(point, delta(Direction::West)) };
        };

    while(any_empty(field))
    {
        std::vector<Point> to_modify;
        for(auto& p : field)
        {
            if(p.second.value == MapValue::Value::Oxygen)
            {
                auto other = other_points(p.first);
                std::copy (other.begin(), other.end(), std::back_inserter(to_modify));
            }
        }

        for(auto& point : to_modify)
        {
            auto& v = field[point];
            if(v.value == MapValue::Value::Empty)
            {
                v.value = MapValue::Value::Oxygen;
            }
        }        

        ++minute;
        robot.print();
        std::cout << "Minute=" << minute << std::endl;

        // int value;
        // std::cin >> value;
    }
    std::cout << "Minutes = " << minute << std::endl;

    return 0;
}
