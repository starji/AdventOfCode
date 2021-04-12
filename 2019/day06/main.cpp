#include <iostream>
#include <locale>
#include <map>

#include "Advent.hpp"




int main()
{
  auto& input = std::cin;
  input.imbue(std::locale(input.getloc(), new Advent::ctype));

  // Key = body
  // Value = bodies directly orbiting
  std::map<std::string, std::string> orbits;

  auto data = Advent::read<std::string>(input);

  for (auto iter = data.begin(); iter != data.end(); ++iter)
    {
      auto body = *iter;
      ++iter;
      auto orbiter = *iter;
      orbits[orbiter] = body;
    }

  std::size_t sum = 0;

  for (auto& element : orbits)
    {
      std::size_t count = 1;
      auto iter = orbits.find(element.second);
      while(iter != orbits.end())
	{
	  count++;
	  iter = orbits.find(iter->second);
	}

      //      std::cout << element.first << "orbits=" << count << std::endl;
      sum += count;
    }

  std::cout << "Total=" << sum << std::endl;


  std::vector<std::string> you_bodies;
  auto you_iter = orbits.find("YOU");
  while(you_iter != orbits.end())
    {
      you_bodies.push_back(you_iter->second);
      you_iter = orbits.find(you_iter->second);
    }


  std::vector<std::string> san_bodies;
  auto san_iter = orbits.find("SAN");
  while(san_iter != orbits.end())
    {
      san_bodies.push_back(san_iter->second);
      san_iter = orbits.find(san_iter->second);
    }



  bool done = false;
  for (auto you_iter = you_bodies.begin(); you_iter != you_bodies.end() && !done; ++you_iter)
    {
      for(auto san_iter = san_bodies.begin(); san_iter != san_bodies.end() && !done; ++san_iter)
	{
	  if (*you_iter == *san_iter)
	    {
	      done = true;
	      std::cout << std::distance(you_bodies.begin(), you_iter) << " "
			<< std::distance(san_bodies.begin(), san_iter) << std::endl;
	    }
	}
    }


  return 0;
}
