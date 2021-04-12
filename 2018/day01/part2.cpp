#include <set>
#include <iostream>
#include <fstream>

int main(int argc, char* argv[])
{
    int working = 0;
    std::set<int> previous;

    std::ifstream input (argv[1]);

    while (previous.insert(working).second || input.eof ())
    {
        int next = 0;
        if (input.eof ())
        {
            std::cout << "reached eof" << std::endl;
            input.clear();
            input.seekg(0);
        }
        input >> next;
        working += next;
    }
    std::cout << working;
}
