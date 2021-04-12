#include <iostream>
#include <locale>
#include <map>
#include <iterator>
#include <cctype>
#include <algorithm>
#include <cstdlib>
#include <set>
#include <cmath>
#include <cstdint>

#include "Advent.hpp"
#include "Intcode.hpp"

#include <boost/range/adaptor/map.hpp>

using Vector = std::tuple<int, int, int>;

const Vector ZERO_V = { 0, 0, 0 };

using State = std::array<std::pair<Vector, Vector>, 4>;

using Dimension = std::array<std::pair<int, int>, 4>;

using VelocityState = std::array<Vector, 4>;

void print(Vector p, Vector v)
{
    std::cout << '<' << std::get<0>(p) << ',' << std::get<1>(p) << ',' << std::get<2>(p) << '>'
              << "  "
              << '<' << std::get<0>(v) << ',' << std::get<1>(v) << ',' << std::get<2>(v) << '>'
              << std::endl;
}

void printState(State s)
{
    print(s[0].first, s[0].second);
    print(s[1].first, s[1].second);
    print(s[2].first, s[2].second);
    print(s[3].first, s[3].second);
}

int energy(Vector p)
{
    return std::abs(std::get<0>(p)) + std::abs(std::get<1>(p)) + std::abs(std::get<2>(p));
}

inline int cmp(int a, int b)
{
    return std::clamp(b - a, -1, 1);
}

int main()
{
  auto& input = std::cin;
  input.imbue(std::locale(input.getloc(), new Advent::ctype));

  Vector m1 = {6, 10, 10};
  Vector m2 = {-9, 3, 17};
  Vector m3 = {9, -4, 14};
  Vector m4 = {4, 14, 4};

  // 2772
  // Vector m1 = {-1, 0, 2};
  // Vector m2 = {2, -10, -7};
  // Vector m3 = {4, -8, 8};
  // Vector m4 = {3, 5, -1};

//   Vector m1 = {-8, -10, 0 };
//   Vector m2 = {5, 5, 10 };
//   Vector m3 = { 2, y=-7, z=3>
// <x=9, y=-8, z=-3>

  Dimension dim_x { std::pair<int, int>(std::get<0>(m1), 0),
                    std::pair<int, int>(std::get<0>(m2), 0),
                    std::pair<int, int>(std::get<0>(m3), 0),
                    std::pair<int, int>(std::get<0>(m4), 0) };;
  Dimension dim_y { std::pair<int, int>(std::get<1>(m1), 0),
                    std::pair<int, int>(std::get<1>(m2), 0),
                    std::pair<int, int>(std::get<1>(m3), 0),
                    std::pair<int, int>(std::get<1>(m4), 0) };;
  Dimension dim_z { std::pair<int, int>(std::get<2>(m1), 0),
                    std::pair<int, int>(std::get<2>(m2), 0),
                    std::pair<int, int>(std::get<2>(m3), 0),
                    std::pair<int, int>(std::get<2>(m4), 0) };;

  std::map<Dimension, std::int64_t> previous;
  previous[dim_y] = 0;

  std::int64_t index = 0;
  do
  {
      for(auto& y : dim_y)
      {
          y.second = y.second +
              cmp(y.first, dim_y[0].first) +
              cmp(y.first, dim_y[1].first) +
              cmp(y.first, dim_y[2].first) +
              cmp(y.first, dim_y[3].first);
      }
      for(auto& y : dim_y)
      {
          y.first = y.first + y.second;
      }
      ++index;

      auto result = previous.insert(std::make_pair(dim_y, index));
      if(result.second == false)
      {
          std::cout << "Previous found at: " << index << " first seen on " << result.first->second << std::endl;
          for(auto& y : dim_y)
          {
              std::cout << y.first << ',' << y.second << "  ";
          }
          std::cout << std::endl;
          break;
      }

  } while(true);


  return 0;
}
