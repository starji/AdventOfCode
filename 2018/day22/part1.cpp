#include <array>
#include <iostream>

// Cave is made up of regions

// Region = x,y coord + type
//  type = rocky,narrow,wet

// cave entrance at 0,0

// type determined by erosion level

// erosion level determined by geologic index.

// geologic index determined by the following:

//  if (position is 0,0) { index = 0; }
//  else if (position is target) { index = 0; }
//  else if (position.y == 0) { index = position.x * 16807; }
//  else if (position.x == 0) { index = position.y * 48271; }
//  else
//  {
//     index = erosion_level (position.x - 1, position.y) * erosion_level (position.x, position.y - 1);
//  }

// erosion level = (geologic index + depth) % 20183

//  rocky type = erosion level % 3 == 0;
//  wet type = erosion level % 3 == 1;
//  narrow = erosion level % 3 == 2;

enum class Type
{
    Rocky, Wet, Narrow
};

const std::size_t DEPTH = 510;
const std::size_t TARGET_X = 10;
const std::size_t TARGET_Y = 10;
// const std::size_t DEPTH = 11541;
// const std::size_t TARGET_X = 14;
// const std::size_t TARGET_Y = 778;
using LevelMap = std::array<std::array<std::size_t, TARGET_Y + 1>, TARGET_X + 1>;
using TypeMap = std::array<std::array<Type, TARGET_Y + 1>, TARGET_X + 1>;

inline std::size_t level (std::size_t index)
{
    return (index + DEPTH) % 20183;
}

std::size_t index (const LevelMap& map, std::size_t x, std::size_t y)
{
    if (x == 0 && y == 0)
    {
        return 0;
    }
    else if (x == (map.size () - 1) && y == (map[x].size () - 1))
    {
        return 0;
    }
    else if (y == 0)
    {
        return x * 16807;
    }
    else if (x == 0)
    {
        return y * 48271;
    }
    else
    {
        return map[x-1][y] * map[x][y-1];
    }
}

enum class Tool
{
    Neither, Torch, Pick
};

// struct Step
// {
//     Position start;
//     Position end;
//     Tool current;
//     std::size_t duration;
// };

void print_map (const TypeMap& map)
{
    for (auto y = 0; y < map[0].size (); ++y)
    {
        for (auto x = 0; x < map.size (); ++x)
        {
            switch (map[x][y])
            {
                case Type::Rocky:
                    std::cout << '.';
                    break;
                case Type::Wet:
                    std::cout << '=';
                    break;
                case Type::Narrow:
                    std::cout << '|';
                    break;
            }
        }
        std::cout << std::endl;
    }
}

int main()
{
    LevelMap level_map;
    TypeMap map;

    std::size_t danger = 0;

    for (auto x = 0; x < level_map.size (); ++x)
    {
        for (auto y = 0; y < level_map[x].size (); ++y)
        {
            level_map[x][y] = level (index (level_map, x, y));
        }
    }

    for (auto x = 0; x < map.size (); ++x)
    {
        for (auto y = 0; y < map[x].size (); ++y)
        {
            map[x][y] = static_cast<Type>(level_map[x][y] % 3);
        }
    }

    print_map (map);

    return 0;
}
