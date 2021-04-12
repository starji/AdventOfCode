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
#include <queue>
#include <atomic>

#include "Advent.hpp"
#include "Intcode.hpp"

using Point = std::pair<int, int>;
using Field = std::map<Point, char>;

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


Field step(Field input)
{
    Field new_field;

    for(auto& p : input)
    {
        auto cardinal = cardinalPoints(p.first);
        int bug_count = 0;
        for(auto& point : cardinal)
        {
            auto iter = input.find(point);
            if(iter != input.end())
            {
                if(iter->second == '#')
                {
                    bug_count++;
                }
            }
        }

        char next_state = p.second;
        switch(p.second)
        {
            case '#':
            {
                if(bug_count != 1)
                {
                    next_state = '.';
                }
            }
            break;
            case '.':
            {
                if(bug_count > 0 && bug_count < 3)
                {
                    next_state = '#';
                }
            }
            break;
        }

        new_field[p.first] = next_state;
    }

    return new_field;
}

Field readInput(std::string filename)
{
    Field result;
    
    std::ifstream input(filename);

    int index = 0;

    auto iter = std::istream_iterator<char>(input);
    while(iter != std::istream_iterator<char>())
    {
        result[{index % 5, index / 5}] = *iter;
        ++index;
        ++iter;
    }

    return result;
}

int score(Field field)
{
    int score = 0;
    for(auto& p : field)
    {
        if(p.second == '#')
        {
            score += std::pow(2, p.first.second * 5 + p.first.first);
        }
    }
    return score;
}

void print(Field field)
{
    Advent::printMap(field.begin(), field.end(),
                     [](auto c)
                     {
                         return c;
                         // if(c == '.') { return ' '; }
                         // if(c == '#') { return '*'; }
                         // else { return '?'; }
                     });
}

using RField = std::array<Field, 20000>;

int countBugsOnSide(const Field& check, Direction d)
{
    int bug_count = 0;
    switch(d)
    {
        case Direction::North:
        {
            for(auto i = 0; i < 5; ++i)
            {
                if(check.at({i, 0}) == '#') { bug_count++; }
            }
        }
        break;
        case Direction::South:
        {
            for(auto i = 0; i < 5; ++i)
            {
                if(check.at({i, 4}) == '#') { bug_count++; }
            }
        }
        break;
        case Direction::East:
        {
            for(auto i = 0; i < 5; ++i)
            {
                if(check.at({4, i}) == '#') { bug_count++; }
            }
        }
        break;
        case Direction::West:
        {
            for(auto i = 0; i < 5; ++i)
            {
                if(check.at({0, i}) == '#') { bug_count++; }
            }
        }
        break;
    }
    return bug_count;
}

int countBugs(Point p, const Field& lower, const Field& current, const Field& upper)
{
    int bug_count = 0;

    auto cardinal = cardinalPoints(p);
    for(auto c : cardinal)
    {
        if(c.first < 0)
        {
            if (upper.at({1, 2}) == '#') { bug_count++; }
        }
        else if(c.first >= 5)
        {
            if (upper.at({3, 2}) == '#') { bug_count++; }
        }
        else if(c.second < 0)
        {
            if (upper.at({2, 1}) == '#') { bug_count++; }
        }
        else if(c.second >= 5)
        {
            if (upper.at({2, 3}) == '#') { bug_count++; }
        }
        else if(c.first == 2 && c.second == 2)
        {
            // here we go
            if(p.first < c.first)
            {
                bug_count += countBugsOnSide(lower, Direction::West);
            }
            else if(p.first > c.first)
            {
                bug_count += countBugsOnSide(lower, Direction::East);
            }
            else if(p.second < c.second)
            {
                bug_count += countBugsOnSide(lower, Direction::North);
            }
            else if(p.second > c.second)
            {
                bug_count += countBugsOnSide(lower, Direction::South);
            }
        }
        else
        {
            if(current.at(c) == '#') { bug_count++; }
        }
    }
    return bug_count;
}

Field middle(const Field& lower, const Field& current, const Field& upper)
{
    Field new_field;

    for(auto& p : current)
    {
        if(p.first.first == 2 && p.first.second == 2)
        {
            new_field[p.first] = p.second;
        }
        else
        {
            int bug_count = countBugs(p.first, lower, current, upper);

            if(p.second == '#' && bug_count != 1)
            {
                new_field[p.first] = '.';
            }
            else if(p.second == '.' && (bug_count == 1 || bug_count == 2))
            {
                new_field[p.first] = '#';
            }
            else
            {
                new_field[p.first] = p.second;
            }
        }
    }

    return new_field;
}


Field zeroField()
{
    Field result;
    for(auto i = 0; i < 25; ++i)
    {
        result[{i % 5, i / 5}] = '.';
    }
    return result;
}

RField step(RField rfield)
{
    RField new_field;

    new_field[0] = zeroField();
    new_field[rfield.size() - 1] = zeroField();

    for(auto i = 1; i < rfield.size() - 1; ++i)
    {
        new_field[i] = middle(rfield[i - 1], rfield[i], rfield[i+1]);
    }

    return new_field;
}

int main(int argc, char* argv[])
{
    RField fields;
    for(auto& f : fields)
    {
        f = zeroField();
    }
    auto mid = fields.size() / 2;
    fields[mid] = readInput("input.txt");

    std::cout << "Middle=" << mid << std::endl;

    // print(fields[mid-1]);
    // print(fields[mid]);
    // print(fields[mid+1]);

    // std::cout << std::endl;

    int index = 0;
    while(index++ < 200)
    {
        fields = step(fields);
        // print(fields[mid-1]);
        // print(fields[mid]);
        // print(fields[mid+1]);

        // std::cout << std::endl;

        
    }

    int total_bugs = 0;
    for(auto& f : fields)
    {
        total_bugs += std::count_if(f.begin(), f.end(), [](auto p) { return p.second == '#'; });
    }

    std::cout << "Total bugs=" << total_bugs << std::endl;
    return 0;
}
