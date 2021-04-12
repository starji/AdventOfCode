#include <iostream>
#include <iterator>

int fuel(int mass)
{
  return (mass / 3) - 2;
}

int totalFuel(int mass)
{
  auto f = fuel(mass);
  return f > 0 ? mass + totalFuel(f) : mass;  
}

int main(int argc, char* argv[])
{
  auto iter = std::istream_iterator<int>{std::cin};
  while (iter != std::istream_iterator<int>{}) {
    std::cout << totalFuel(fuel(*iter)) << std::endl;
    ++iter;
  }
  return 0;
}
