#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>


std::string collapse (std::string chain)
{
    auto iter = std::adjacent_find (chain.begin (), chain.end (),
                                    [](char a, char b)
                                    {
                                        return (std::max(a,b) - std::min(a,b)) == ('a'-'A');
                                    });
    while (iter != chain.end ())
    {
        chain.erase (iter, iter + 2);
        iter = std::adjacent_find (chain.begin (), chain.end (),
                                    [](char a, char b)
                                    {
                                        return (std::max(a,b) - std::min(a,b)) == ('a'-'A');
                                    });
    }          

    return chain;
}

std::string strip (std::string chain, char a, char b)
{
    std::string result;

    std::copy_if (chain.begin (),
                  chain.end (),
                  std::back_inserter(result),
                  [=] (char check) { return check != a && check != b; });

    return result;
}

int main(int argc, char* argv[])
{
    std::ifstream input (argv[1]);

    std::string chain;
    std::getline(input, chain);

    for (auto i = 'A'; i <= 'Z'; ++i)
    {
        auto collapsed = collapse (strip (chain, i, i+('a'-'A')));

        std::cout << i << " : " << collapsed.size () << std::endl;
    }

    return 0;
}
