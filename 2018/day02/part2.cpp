#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

bool compare(std::string a, std::string b)
{
    auto mismatch_pair = std::mismatch(a.begin (), a.end (), b.begin ());
    ++mismatch_pair.first;
    ++mismatch_pair.second;

    return std::mismatch(mismatch_pair.first, a.end (), mismatch_pair.second).first == a.end ();
}

int main(int argc, char* argv[])
{
    std::ifstream input(argv[1]);

    std::vector<std::string> list;

    while(input.good())
    {
        std::string line;
        std::getline(input, line);
        list.push_back(line);
    }

    auto iter = list.begin ();
    while (iter != list.end ())
    {
        auto next = iter;
        ++next;

        while (next != list.end ())
        {
            if (compare (*iter, *next))
            {
                std::string result;
                std::cout << *iter << std::endl;
                std::cout << *next << std::endl;

                std::set_intersection(iter->begin (), iter->end (),
                                      next->begin (), next->end (),
                                      std::back_inserter(result));
                std::cout << result << std::endl;
            }
            ++next;
        }
        ++iter;
    }
}
