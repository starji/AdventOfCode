#include <iostream>
#include <string>
#include <algorithm>

int main()
{
    while(std::cin.good ())
    {
        std::string line;
        std::getline(std::cin, line);
        std::sort(line.begin (), line.end ());
        std::cout << line << std::endl;
    }

    return 0;
}
