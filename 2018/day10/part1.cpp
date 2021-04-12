#include <iostream>
#include <fstream>
#include <vector>
#include <limits>
#include <array>
#include <algorithm>

#include "advent.hpp"

struct Star
{
    std::pair<int, int> position;
    std::pair<int, int> velocity;
};

int min_x (const std::vector<Star>& stars)
{
    int min_x = std::numeric_limits<int>::max ();
    for (auto i : stars)
    {
        min_x = std::min (i.position.first, min_x);
    }
    return min_x;
}

int max_x (const std::vector<Star>& stars)
{
    int max_x = std::numeric_limits<int>::min ();
    for (auto i : stars)
    {
        max_x = std::max (i.position.first, max_x);
    }
    return max_x;
}


int min_y (const std::vector<Star>& stars)
{
    int min_y = std::numeric_limits<int>::max ();
    for (auto i : stars)
    {
        min_y = std::min (i.position.second, min_y);
    }
    return min_y;
}

int max_y (const std::vector<Star>& stars)
{
    int max_y = std::numeric_limits<int>::min ();
    for (auto i : stars)
    {
        max_y = std::max (i.position.second, max_y);
    }
    return max_y;
}


void print (const Star& star)
{
    std::cout << /*"pos=" << */star.position.first << "," << star.position.second
//              << " vel=" << star.velocity.first << "," << star.velocity.second
              << std::endl;
}

const std::size_t WIDTH = 180;
const std::size_t HEIGHT = 60;

void print (const std::vector<Star>& stars)
{
    std::array<std::array<char, WIDTH>, HEIGHT> sky = { 0 };

    std::size_t offset_x = 80;
    std::size_t offset_y = 100;

    for (auto& s : stars)
    {
        int x = s.position.first - offset_x;// + (WIDTH/2);
        int y = s.position.second - offset_y;// + (HEIGHT/2);

        x = std::clamp (x, 0, (int) (WIDTH-1));
        y = std::clamp (y, 0, (int) (HEIGHT-1));

        if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
        {
            sky[y][x] = '#';
        }

        // std::size_t x = (s.position.first * scale_width) + (WIDTH/2);
        // x = std::clamp (x, 0ul, WIDTH-1);
        // std::size_t y = (s.position.second * scale_height) + (HEIGHT/2);
        // y = std::clamp (y, 0ul, HEIGHT-1);
    }

    for (auto& row : sky)
    {
        for (auto& col : row)
        {
            if (col > 0) { std::cout << col; } else { std::cout << '.'; }
        }
        std::cout << std::endl;
    }

    std::cout << "------------------------------" << std::endl;
}

void advance (std::vector<Star>& stars)
{
    for (auto& s : stars)
    {
        s.position.first += s.velocity.first;
        s.position.second += s.velocity.second;
    }
}

void revert (std::vector<Star>& stars)
{
    for (auto& s : stars)
    {
        s.position.first -= s.velocity.first;
        s.position.second -= s.velocity.second;
    }
}

void advance (std::vector<Star>& stars, int steps)
{
    auto count = std::abs (steps);
    if (steps >= 0)
    {
        while (count-- > 0)
        {
            advance (stars);
        }
    }
    else
    {
        while (count-- > 0)
        {
            revert (stars);
        }
    }
}

int main (int argc, char* argv[])
{
    std::ifstream input (argv[1]);

    std::size_t iteration = std::stoul (argv[2]);

    input.imbue(std::locale(input.getloc(), new advent::ctype));

    std::vector<Star> stars;
    
    do
    {
        Star star;
        input >> star.position.first >> star.position.second
              >> star.velocity.first >> star.velocity.second;
        stars.push_back (std::move (star));
        input.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    } while (input.good ());
    stars.pop_back ();

    while (iteration-- > 0)
    {
        advance (stars);
    }

    print (stars);

    // for (auto& s : stars)
    // {
    //     print (s);
    // }
    
    return 0;
}
