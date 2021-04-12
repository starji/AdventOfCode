#include <iostream>
#include <locale>
#include <map>
#include <iterator>
#include <cctype>
#include <algorithm>
#include <cstdlib>
#include <set>
#include <cmath>

#include "Advent.hpp"
#include "Intcode.hpp"

std::pair<int, int> indexToCoord(int index)
{
  return { index % 20, index / 20 };
}

std::pair<int, int> delta(std::pair<int, int> coord1, std::pair<int, int> coord2)
{
  return { coord1.first - coord2.first, coord1.second - coord2.second };
}

int gcd (int a, int b)
{
  auto ap = std::abs(a);
  auto bp = std::abs(b);
  auto x = std::max(ap,bp);
  auto y = std::min(ap,bp);
  if (y > 0)
    {
      return gcd (y, x % y);
    }
  else
    {
      return x;
    }
}

using Asteroid = std::tuple<double, double, std::pair<int, int>>;

void printAsteroid(Asteroid& a)
{
  std::cout << std::get<0>(a) << " " << std::get<1>(a) << " " << std::get<2>(a).first << ',' << std::get<2>(a).second << std::endl;
}

int main()
{
  auto& input = std::cin;
  input.imbue(std::locale(input.getloc(), new Advent::ctype));

  // std::array<std::array<char, 20> 20> map
  //   {
  //    { .###.###.###.#####.#   },
  //    { #####.##.###..###..# },
  //    { .#...####.###.###### },
  //    { ######.###.####.#### },
  //    { #####..###..######## },
  //    { #.##.###########.#.# },
  //    { ##.###.######..#.#.# },
  //    { .#.##.###.#.####.### },
  //    { ##..#.#.##.######### },
  //    { ###.#######.###..##. },
  //    { ###.###.##.##..####. },
  //    { .##.####.##########. },
  //    { #######.##.###.##### },
  //    { #####.##..####.##### },
  //    { ##.#.#####.##.#.#..# },
  //    { ###########.#######. },
  //    { #.##..#####.#####..# },
  //    { #####..#####.###.### },
  //    { ####.#.############. },
  //    { ####.#.#.##########. },
  //   }

  std::vector<char> map { std::istream_iterator<char>{std::cin}, std::istream_iterator<char>{} };
  
  // for(auto i = 0; i < 20; ++i)
  //   {
  //     for(auto j = 0; j < 20; ++j)
  // 	{
  // 	  std::cout << map[i*20+j];
  // 	}
  //     std::cout << std::endl;
  //   }
  
  std::vector<std::pair<int, int> > points;
  for(auto i = 0; i < map.size(); ++i)
    {
      if (map[i] == '#')
	{
	  points.push_back(indexToCoord(i));
	}
    }

  auto p = std::make_pair (8, 16);

  std::vector<std::pair<int, int>> deltas;


  struct Compare
  {
    bool operator()(Asteroid& a, Asteroid& b)
    {
      if(std::get<0>(a) < std::get<0>(b))
	{
	  return true;
	}
      else if(std::get<0>(a) == std::get<0>(b))
	{
	  return std::get<1>(a) > std::get<1>(b);
	}
      else
	{
	  return false;
	}
    }
  };


  std::map<double, std::vector<Asteroid>> asteroids;
  std::set<double> keys;
  for(auto& other: points)
    {
      auto d = delta(other, p);
      // if (d.first != 0 && d.second != 0)
      // 	{
      // 	  auto g = gcd(d.first, d.second);
      // 	  deltas.push_back({ d.first/g, d.second/g });
      // 	}
      // else
      // 	{
      // 	  deltas.push_back(d);
      // 	}
      auto angle = std::atan2(static_cast<double>(d.first), static_cast<double>(d.second));
      auto hypot = std::hypot(static_cast<double>(d.first),static_cast<double>(d.second));

      asteroids[angle].push_back({angle, hypot, other});
      keys.insert(angle);
    }

  int index = 0;
  auto key_iter = keys.rbegin();
  while(!asteroids.empty())
    {
      auto angle_iter = asteroids.find(*key_iter);
      if(angle_iter != asteroids.end())
	{
	  auto asteroid_iter = std::min_element(angle_iter->second.begin(), angle_iter->second.end(),
						[](auto& a, auto& b)
						{
						  return std::get<1>(a) < std::get<1>(b);
						});
	  if(asteroid_iter != angle_iter->second.end())
	    {
	      // remove
	      std::cout << ++index << ' ';
	      printAsteroid(*asteroid_iter);
	      angle_iter->second.erase(asteroid_iter);
	    }

	  if(angle_iter->second.empty())
	    {
	      asteroids.erase(angle_iter);
	    }
	}

      // loop keys
      ++key_iter;
      if(key_iter == keys.rend())
	{
	  key_iter = keys.rbegin();
	}
    }
  
  // std::vector<std::pair<double, std::pair<int, int> > > next;
  // auto iter = keys.rbegin();
  // while(!angles.empty())
  //   {
  //     auto iter_pair = angles.equal_range(*iter);
  //     auto min_iter = std::min_element(iter_pair.first, iter_pair.second,
  // 				       [](auto& x, auto& y)
  // 				       {
  // 					 return
  // 					   std::hypot(static_cast<double>(x.second.first), static_cast<double>(x.second.second)) <
  // 					   std::hypot(static_cast<double>(y.second.first), static_cast<double>(y.second.second));
  // 				       });
  //     if (min_iter != angles.end())
  // 	{
  // 	  next.push_back(std::make_pair(min_iter->first, min_iter->second));
  // 	  angles.erase(min_iter);
  // 	}

  //     ++iter;
  //     if (iter == keys.rend())
  // 	{
  // 	  iter = keys.rbegin();
  // 	}
  //   }

  // for(auto& d : next)
  //   {
  //     std::cout << d.first << " "
  // 		<< std::hypot(static_cast<double>(d.second.first),static_cast<double>(d.second.second)) << " "
  // 		<< d.second.first << ',' << d.second.second << std::endl;
  //   }

  // 3.14159 = north

  // 1.5708 = east

  // 0 = south

  // -1.5708 = west


#if 0 // part 1

  for(auto& p : points)
    {
      std::vector<std::pair<int, int>> deltas;
      for(auto& other: points)
	{
	  auto d = delta(other, p);
	  if (d.first != 0 && d.second != 0)
	    {
	      auto g = gcd(d.first, d.second);
	      deltas.push_back({ d.first/g, d.second/g });
	    }
	  else
	    {
	      deltas.push_back(d);
	    }
	}

      std::sort(deltas.begin(), deltas.end());

      std::set<std::pair<int, int>> culled_deltas;

      auto iter = std::find_if(deltas.begin(), deltas.end(),
			       [](auto d) { return d.first > 0 && d.second == 0; });
      if (iter != deltas.end())
	{
	  culled_deltas.insert(*iter);
	}
      iter = std::find_if(deltas.begin(), deltas.end(),
			  [](auto d) { return d.first < 0 && d.second == 0; });
      if (iter != deltas.end())
	{
	  culled_deltas.insert(*iter);
	}
      iter = std::find_if(deltas.begin(), deltas.end(),
			  [](auto d) { return d.first == 0 && d.second > 0; });
      if (iter != deltas.end())
	{
	  culled_deltas.insert(*iter);
	}
      iter = std::find_if(deltas.begin(), deltas.end(),
			  [](auto d) { return d.first == 0 && d.second < 0; });
      if (iter != deltas.end())
	{
	  culled_deltas.insert(*iter);
	}

      for(auto& d : deltas)
	{
	  if(d.first != 0 && d.second != 0)
	    {
	      culled_deltas.insert(d);
	    }
	}

      std::cout << culled_deltas.size () << ": " << p.first << ',' << p.second << std::endl;
    }
#endif

  

  return 0;
}
