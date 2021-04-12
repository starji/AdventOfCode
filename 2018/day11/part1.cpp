#include <iostream>
#include <limits>

const std::size_t SERIAL_ID = 5719;

template <std::size_t Digit> int digit (int value) { return digit<Digit-1>(value / 10); }
template <> int digit<1>(int value) { return value % 10; }

int power_level (std::size_t x, std::size_t y, std::size_t serial_id)
{
    auto rack_id = x + 10;
    return digit<3>((rack_id * y + serial_id) * rack_id) - 5;
}

int grid_level (std::size_t x, std::size_t y, std::size_t grid, std::size_t serial_id)
{
    if (grid == 0)
    {
        return power_level (x, y, serial_id);
    }
    else
    {
        int level = grid_level (x, y, grid-1, serial_id);
        for (auto i = 0; i <= grid; ++i)
        {
            level += power_level (x + i, y + grid, serial_id);
        }
        for (auto i = 0; i < grid; ++i)
        {
            level += power_level (x + grid, y + i, serial_id);
        }
        return level;
    }
    // return power_level (x, y, serial_id) +
    //     power_level (x + 1, y, serial_id) +
    //     power_level (x + 2, y, serial_id) +
    //     power_level (x, y + 1, serial_id) +
    //     power_level (x + 1, y + 1, serial_id) +
    //     power_level (x + 2, y + 1, serial_id) +
    //     power_level (x, y + 2, serial_id) +
    //     power_level (x + 1, y + 2, serial_id) +
    //     power_level (x + 2, y + 2, serial_id);
}

int main ()
{
    std::size_t x = 0;
    std::size_t y = 0;
    int value = std::numeric_limits<int>::min ();;

    for (auto grid = 0; grid < 300; ++grid)
    {
        for (auto i = 1; i <= 300 - grid; ++i)
        {
            for (auto j = 1; j <= 300 - grid; ++j)
            {
                auto grid_power = grid_level (i, j, grid, SERIAL_ID);
                if (grid_power > value)
                {
                    x = i; y = j; value = grid_power;
                }
            }
        }
        std::cout << "at " << x << ',' << y << ',' << grid << " power=" << value << std::endl;
        value = std::numeric_limits<int>::min ();
    }

    return 0;
}
