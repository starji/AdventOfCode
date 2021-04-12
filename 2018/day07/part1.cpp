#include <iostream>
#include <vector>
#include <fstream>
#include <map>
#include <algorithm>
#include <string>
#include <set>

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
            custom_table['.'] |= space;
            for (auto i = 'a' ; i <= 'z'; ++i)
            {
                custom_table[i] |= space;
            }
        }
        
        mask custom_table[table_size];
    };
}

struct Step
{
    Step(char name)
        : name (name)
    {}

    

    char name;
    std::vector<char> prereq;
};

int main (int argc, char* argv[])
{
    std::ifstream input (argv[1]);

    input.imbue(std::locale(std::cin.getloc(), new advent::ctype));

    std::map<char, std::vector<char>> dependencies;
    for (auto i = 'A'; i <= 'Z'; ++i)
    {
        dependencies[i].clear ();
    }

    do
    {
        char from = 0, to = 0;
        input >> from >> to;
        if (from > 0)
        {
            dependencies[to].push_back(from);
        }
        input.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    } while(input.good());


    while (!dependencies.empty ())
    {
        std::set<char> available;
        for (auto& p : dependencies)
        {
            if (p.second.empty ())
            {
                available.insert(p.first);
            }
        }

        if (available.size () > 0)
        {
            char rule = *available.begin ();
            std::cout << rule;
            dependencies.erase (rule);

            for (auto& p : dependencies)
            {
                auto iter = std::find(p.second.begin (), p.second.end (), rule);
                if (iter != p.second.end ())
                {
                    p.second.erase (iter);
                }
            }
        }
    }

    std::cout << std::endl;

    return 0;
}
