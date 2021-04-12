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

void finished(std::map<char, std::vector<char>>& dependencies, char task)
{
    for (auto& p : dependencies)
    {
        auto iter = std::find(p.second.begin (), p.second.end (), task);
        if (iter != p.second.end ())
        {
            p.second.erase (iter);
        }
    }
}

struct ElfPool
{
    ElfPool (std::map<char, std::vector<char>>& dependencies)
        : dependencies(dependencies)
    {}

    bool assign(char task)
    {
        if (elves.size () >= 5)
        {
            return false;
        }

        elves[task] = 61 + (task - 'A');
        return true;
    }

    void advance()
    {
        auto iter = elves.begin ();
        while (iter != elves.end ())
        {
            auto current = iter;
            ++iter;
            
            current->second--;
            if (current->second == 0)
            {
                finished(dependencies, current->first);
                elves.erase (current);
            }
        }
    }

    void printStatus ()
    {
        for (auto& e : elves)
        {
            std::cout << e.first << ":" << e.second << "   ";
        }
        std::cout << std::endl;
    }

    bool isWorking () { return elves.empty () == false; }

    std::map<char, std::vector<char>>& dependencies;
    std::map<char, std::size_t> elves;
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

    std::vector<char> assigned;
    ElfPool pool(dependencies);
    std::size_t second = 0;
    while (!dependencies.empty () || pool.isWorking ())
    {
        std::set<char> available;
        for (auto& p : dependencies)
        {
            if (p.second.empty ())
            {
                available.insert(p.first);
            }
        }

        for (auto& a : available)
        {
            if (pool.assign (a))
            {
                assigned.push_back(a);
                dependencies.erase (a);
            }
        }

        std::cout << second << ": ";
        pool.printStatus ();

        pool.advance ();
        second++;
    }

    pool.printStatus ();
    std::cout << second << std::endl;

    for (auto& a : assigned)
    {
        std::cout << a;
    }
    std::cout << std::endl;

    return 0;
}
