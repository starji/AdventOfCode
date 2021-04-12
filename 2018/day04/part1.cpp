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

    using shift_map = std::map<std::size_t, std::map<std::size_t, std::size_t> >;
}



int main()
{
    advent::ctype custom_ctype;
    advent::shift_map shifts;

    std::cin.imbue(std::locale(std::cin.getloc(), &custom_ctype));

    std::size_t year, month, day, hour, minute, guard_id;
    std::string command;

    std::size_t shift_start;

    do
    {
        std::cin >> year >> month >> day >> hour >> minute >> command;

        if (command == "Guard")
        {
            std::cin >> guard_id;
//            std::cout << std::endl << "Guard at " << guard_id << " : ";
        }
        else if (command == "falls")
        {
            shift_start = minute;
        }
        else
        {
            for (std::size_t i = shift_start; i < minute; ++i)
            {
                shifts[guard_id][i]++;
            }
//            std::cout << "sleeps from " << shift_start << " to " << minute << " ";
        }

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    } while(std::cin.good());

    for (auto& p : shifts)
    {
        std::size_t sum = 0;
        std::size_t most = p.second.begin ()->first;
        std::size_t most_count = p.second.begin ()->second;
        for (auto& mins : p.second)
        {
            if (most_count < mins.second)
            {
                most = mins.first;
                most_count = mins.second;
            }
            sum += mins.second;
        }

        std::cout << "Guard: " << p.first << " sleeping: " << sum << " most: " << most << " most count: " << most_count << std::endl;
    }

    return 0;
}
