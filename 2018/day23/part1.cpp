#include <iostream>
#include <fstream>
#include <vector>
#include <limits>
#include <map>

#include "advent.hpp"

struct Point
{
    Point ()
        : x(0), y(0), z(0), r(0)
    {}
    
    Point (int x, int y, int z, int r)
        : x(x), y(y), z(z), r(r)
    {}

    int x, y, z, r;
};

int distance (Point a, Point b)
{
    return std::abs(a.x - b.x) + std::abs(a.y - b.y) + std::abs(a.z - b.z);
}

std::istream& operator>>(std::istream& in, Point& p)
{
    in >> p.x >> p.y >> p.z >> p.r;
    return in;
}

std::ostream& operator<<(std::ostream& out, const Point& p)
{
    out << '<' << p.x << ',' << p.y << ',' << p.z << "> r=" << p.r;
    return out;
}

int main ()
{
    auto& input = std::cin;
    input.imbue(std::locale(input.getloc(), new advent::ctype));

    std::vector<Point> points;

    do
    {
        Point p;
        input >> p;
        if (input.good ())
        {
            points.push_back (std::move (p));
        }

        // int x, y, z, r;
        // input >> x >> y >> z >> r;
        // points.push_back (Point (x, y, z, r));

        input.ignore (std::numeric_limits<std::streamsize>::max(), '\n');
    } while (input.good());


    std::multimap<std::size_t, Point> scores;
    std::multimap<std::size_t, std::size_t> radius_to_count;

    for (auto& p : points)
    {
        std::size_t count = 0;
        for (auto& pp : points)
        {
            if (distance (p, pp) <= p.r)
            {
                count++;
            }
        }

        scores.insert (std::make_pair (count, p));
        radius_to_count.insert (std::make_pair (p.r, count));
    }


    for (auto& s : radius_to_count)
    {
        std::cout << s.first << ' ' << s.second << std::endl;
    }
}
