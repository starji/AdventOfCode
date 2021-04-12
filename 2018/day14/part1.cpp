#include <iostream>
#include <fstream>
#include <list>
#include <vector>
#include <algorithm>

using ScoreList = std::list<std::size_t>;

ScoreList::iterator loop_advance (ScoreList& scores, ScoreList::iterator iter)
{
    ++iter;
    if (iter == scores.end())
    {
        iter = scores.begin ();
    }
    return iter;
}

ScoreList::iterator loop_advance (ScoreList& scores, ScoreList::iterator iter, std::size_t count)
{
    while (count-- > 0)
    {
        iter = loop_advance (scores, iter);
    }
    return iter;
}

void print_ten (ScoreList::iterator iter)
{
    std::size_t count = 10;
    while (count-- > 0)
    {
        std::cout << *iter;
        ++iter;
    }
    std::cout << std::endl;
}

int main()
{
    std::vector<std::size_t> match { 5, 4, 0, 3, 9, 1 };

//    ScoreList scores { 5, 4, 0, 3, 9, 1 };
    ScoreList scores { 3, 7 };

    ScoreList::iterator second = scores.begin ();
    ScoreList::iterator first = second;
    ++second;

    while (scores.size () < 100000000)
    {
        auto sum = *first + *second;
        if (sum > 9)
        {
            scores.push_back (1);
        }
        scores.push_back (sum % 10);

        first = loop_advance (scores, first, *first + 1);
        second = loop_advance (scores, second, *second + 1);
    }

    auto t1 = scores.begin (), t2 = scores.begin (), t3 = scores.begin (), t4 = scores.begin (), t5 = scores.begin ();

    std::advance (t1, 9);
    std::advance (t2, 5);
    std::advance (t3, 18);
    std::advance (t4, 2018);
    std::advance (t5, 540391);

    print_ten (t1);
    print_ten (t2);
    print_ten (t3);
    print_ten (t4);
    print_ten (t5);

    auto iter = std::search (scores.begin (), scores.end (),
                             match.begin (), match.end ());

    if (iter != scores.end ())
    {
        std::cout << "Found after " << std::distance (scores.begin (), iter) << std::endl;
    }
    else
    {
        std::cout << "Not Found" << std::endl;
    }

    return 0;
}
