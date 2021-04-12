#include <iostream>
#include <vector>
#include <fstream>
#include <map>
#include <algorithm>

namespace advent
{
    struct ctype : public std::ctype<char>
    {
        ctype(std::size_t refs = 0)
            : std::ctype<char>(custom_table, false, refs)
        {
            std::copy(classic_table(), classic_table()+table_size, custom_table);
            custom_table['@'] |= space;
            custom_table[':'] |= space;
            custom_table['#'] |= space;
            custom_table[','] |= space;
            custom_table['['] |= space;
            custom_table[']'] |= space;
            custom_table['-'] |= space;
        }
        
        mask custom_table[table_size];
    };

}

int main(int argc, char* argv[])
{
    advent::ctype custom_ctype;

    std::cin.imbue(std::locale(std::cin.getloc(), &custom_ctype));

    std::vector<std::pair<int, int>> points;

    do
    {
        std::pair<int, int> point;
        std::cin >> point.first >> point.second;
        if (point.first > 0)
            points.push_back(point);
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    } while(std::cin.good());


    std::map<std::size_t, std::size_t> point_count;

    for (auto i = 0; i < 400; ++i)
    {
        for (auto j = 0; j < 400; ++j)
        {
            std::vector<std::size_t> distances;

            std::transform (points.begin (), points.end (), std::back_inserter (distances),
                            [&](std::pair<int, int> point)
                            { return std::abs(point.first - i) + std::abs(point.second - j); });
            auto point_index = std::min_element (distances.begin (), distances.end ()) - distances.begin ();
            if (std::count (distances.begin (), distances.end (), distances[point_index]) == 1)
            {
                point_count[point_index]++;
                if (i == 0 || i == 399 || j == 0 || j == 399)
                    point_count[point_index] += 1000000;
            }
        }
    }

    for (auto& c : point_count)
    {
        std::cout << c.second << " = " << c.first << std::endl;
    }

    return 0;
}
