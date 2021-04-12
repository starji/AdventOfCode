#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>

std::vector<std::string> possible_patterns (std::string generation)
{
    std::vector<std::string> result;

    for (auto i = 0; i < 4; ++i)
    {
        result.push_back (std::string(4-i, '.') + generation.substr(0,1+i));
    }

    for (auto i = 0; i < generation.size (); ++i)
    {
        auto str = generation.substr (i, 5);
        str.resize (5, '.');
        result.push_back (std::move (str));
    }

    return result;
}

int main ()
{
    std::map<std::string, char> rules {
        { "#####", '.' },
        { "####.", '#' },
        { "###.#", '#' },
        { "###..", '#' },
        { "##.##", '.' },
        { "##.#.", '.' },
        { "##..#", '#' },
        { "##...", '#' },
        { "#.###", '.' },
        { "#.##.", '#' },
        { "#.#.#", '.' },
        { "#.#..", '#' },
        { "#..##", '.' },
        { "#..#.", '#' },
        { "#...#", '.' },
        { "#....", '.' },
        { ".####", '.' },
        { ".###.", '#' },
        { ".##.#", '#' },
        { ".##..", '.' },
        { ".#.##", '#' },
        { ".#.#.", '#' },
        { ".#..#", '#' },
        { ".#...", '#' },
        { "..###", '.' },
        { "..##.", '.' },
        { "..#.#", '#' },
        { "..#..", '.' },
        { "...##", '#' },
        { "...#.", '.' },
        { "....#", '.' },
        { ".....", '.' },
    };

    std::string generation = "##.##.#.#...#......#..#.###..##...##.#####..#..###.########.##.....#...#...##....##.#...#.###...#.##";
    int offset = 0;

//    std::cout << generation << std::endl;

    std::uint64_t count = 1000;
    while (count-- > 0)
    {
        auto possible = possible_patterns (generation);
        std::string next;
        std::transform (possible.begin (), possible.end (), std::back_inserter (next),
                        [&](const std::string& str)
                        {
                            return rules.find (str)->second;
                        });

        auto iter = next.begin ();
        while (*iter != '#' && iter != next.end ())
        {
            ++iter;
        }
        offset += 2 - (iter - next.begin ());
        next.erase (next.begin (), iter);

        auto rightpos = next.find_last_of ('#');
        next.erase (rightpos + 1, std::string::npos);

        std::int64_t sum = 0;
        for (auto i = 0; i < generation.size (); ++i)
        {
            if (generation[i] == '#')
            {
                sum += (i - offset);
            }
        }
        std::cout << sum << ": " <<  next << std::endl;

        generation = next;
    }
    
    return 0;
}
