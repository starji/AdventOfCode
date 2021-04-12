#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>

#include "advent.hpp"

struct Point
{
    Point ()
        : x(0), y(0), z(0), w(0)
    {}
    
    Point (int x, int y, int z, int w)
        : x(x), y(y), z(z), w(w)
    {}

    int x, y, z, w;
};

int distance (Point a, Point b)
{
    return std::abs(a.x - b.x) + std::abs(a.y - b.y) + std::abs(a.z - b.z) + std::abs(a.w - b.w);
}

std::istream& operator>>(std::istream& in, Point& p)
{
    in >> p.x >> p.y >> p.z >> p.w;
    return in;
}

std::ostream& operator<<(std::ostream& out, const Point& p)
{
    out << '<' << p.x << ',' << p.y << ',' << p.z << ',' << p.w << '>';
    return out;
}

struct Constellation
{
    std::vector<Point> points;
};

std::ostream& operator<<(std::ostream& out, const Constellation& c)
{
    out << "Constellation:" << std::endl;
    for (auto& p : c.points)
    {
        out << '\t' << p << std::endl;
    }
    return out;
}

Constellation find_constellation (std::vector<Point>& points)
{
    Constellation c;
    c.points.push_back (points.front ());
    points.erase (points.begin ());

    auto lambda = [&] (const Point& a)
                  {
                      return std::find_if (c.points.begin (), c.points.end (),
                                           [&](const Point& b)
                                           {
                                               return distance (a, b) <= 3;
                                           }) != c.points.end ();
                  };
    auto iter = std::find_if (points.begin (), points.end (), lambda);
    while (iter != points.end ())
    {
        c.points.push_back (*iter);
        points.erase (iter);
        iter = std::find_if (points.begin (), points.end (), lambda);
    }

    return c;
}

int main()
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

        input.ignore (std::numeric_limits<std::streamsize>::max(), '\n');
    } while (input.good());

    std::vector<Constellation> constellations;

    do
    {
        constellations.push_back (find_constellation (points));
    } while (!points.empty ());

    for (auto& c: constellations)
    {
        std::cout << c << std::endl;
    }

    std::cout << constellations.size () << std::endl;

    return 0;
}
