#include <iostream>

int main ()
{
    // max_loop = 10551267

    const std::size_t max_loop = 10551267;

    std::size_t first = 0;

    for (std::size_t i = 1; i <= max_loop; ++i)
    {
        if (max_loop % i == 0)
        {
            first += max_loop / i;
        }
    }

    std::cout << first << std::endl;

    // If loop_counter * temp  == max_loop
    //  first += temp;

    // if loop_counter > max_loop
    //  temp++
    //  if temp > max_loop
    //    jump to crazy town?
    
    

}
