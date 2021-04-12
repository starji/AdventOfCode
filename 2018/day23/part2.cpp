#include <iostream>
#include <fstream>
#include <vector>
#include <limits>
#include <map>
#include <algorithm>

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

std::multimap<std::size_t, Point> point_search (const std::vector<Point>& points,
                                                int center_x, int center_y, int center_z,
                                                int box_width, int step)
{
    std::multimap<std::size_t, Point> result;

    auto half = box_width / 2;
    for (int x = center_x - half; x < center_x + half; x += step)
    {
        for (int y = center_y - half; y < center_y + half; y += step)
        {
            for (int z = center_z - half; z < center_z + half; z += step)
            {
                Point test (x, y, z, 0);

                std::size_t count = 0;
                for (auto& p : points)
                {
                    if (distance (test, p) <= p.r)
                    {
                        count++;
                    }
                }

                result.insert (std::make_pair (count, test));
            }
        }
    }

    return result;
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

    int min_x = std::numeric_limits<int>::max ();
    int min_y = std::numeric_limits<int>::max ();
    int min_z = std::numeric_limits<int>::max ();
    int max_x = std::numeric_limits<int>::min ();
    int max_y = std::numeric_limits<int>::min ();
    int max_z = std::numeric_limits<int>::min ();
    for (auto& p : points)
    {
        min_x = std::min (p.x, min_x);
        min_y = std::min (p.y, min_y);
        min_z = std::min (p.z, min_z);
        max_x = std::max (p.x, max_x);
        max_y = std::max (p.y, max_y);
        max_z = std::max (p.z, max_z);
    }
    auto longest_edge = std::max(std::max(max_x - min_x, max_y - min_y), max_z - min_z);
    int center_x = (max_x - min_x) / 2;
    int center_y = (max_y - min_y) / 2;
    int center_z = (max_z - min_z) / 2;
    auto step = longest_edge / 100;

    std::cout << "Incoming search at: " << center_x << ',' << center_y << ',' << center_z
              << " edge=" << longest_edge << " step=" << step << std::endl;
    auto region_map = point_search (points,
                                    center_x,
                                    center_y,
                                    center_z,
                                    longest_edge,
                                    step);
    while (step > 1)
    {
        Point test = region_map.rbegin ()->second;
        longest_edge = longest_edge / 2;
        step = std::max(step / 2, 1); //std::max (longest_edge / 100, 1);

        std::cout << "Incoming search at: " << test.x << ',' << test.y << ',' << test.z
                  << " edge=" << longest_edge << " step=" << step << std::endl;


        region_map = point_search (points, test.x, test.y, test.z, longest_edge, step);
    }

    auto iter = region_map.rbegin ();
    std::size_t count = 10;
    while (count-- > 0)
    {
        std::cout << iter->first << ' ' << iter->second << std::endl;
        ++iter;
    }

}
