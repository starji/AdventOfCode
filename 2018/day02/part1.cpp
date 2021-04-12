#include <iostream>
#include <string>
#include <algorithm>

int main(int argc, char* argv[])
{
    const int target_count = std::stoi(argv[1]);

    std::string line;
    while(std::cin.good())
    {
        std::getline(std::cin, line);
        std::sort(line.begin (), line.end ());

        auto iter = std::adjacent_find(line.begin (), line.end ());
        while (iter != line.end ())
        {
            auto count = std::count(iter, line.end (), *iter);
            if (count != target_count)
            {
                while (count > 0 && iter != line.end ())
                {
                    ++iter;
                    --count;
                }
            }
            else
            {
                std::cout << "1" << std::endl;
                iter = line.end ();
            }
        }
    }

    return 0;
}
