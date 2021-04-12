#include <iostream>
#include <locale>
#include <map>
#include <iterator>
#include <cctype>
#include <algorithm>

#include "Advent.hpp"

int main()
{
  auto& input = std::cin;
  input.imbue(std::locale(input.getloc(), new Advent::ctype));


  using layer = std::array<char, 150>;
  std::vector<layer> layers;

  auto iter = std::istream_iterator<char> {std::cin};
  while(iter != std::istream_iterator<char> {})
    {
      layer next;
      for(auto i = 0; i < 150 && iter != std::istream_iterator<char>{}; ++i)
	{
	  next[i] = *iter;
	  ++iter;
	}
      layers.push_back(next);
    }

  #if 0
  // part 1
  for(auto& l : layers)
    {
      std::sort(l.begin(), l.end());
      auto zero = std::count(l.begin(), l.end(), '0');
      auto ones = std::count(l.begin(), l.end(), '1');
      auto twos = std::count(l.begin(), l.end(), '2');

      std::cout << zero << " " << ones * twos << std::endl;
    }
#endif

  #if 0
  for(auto r = 0; r < 6; ++r)
    {
      for(auto c = 0; c < 25; ++c)
	{
	  auto index = r*25+c;

	  auto iter = std::find_if(layers.begin(), layers.end(),
				   [&](layer& l)
				   {
				     return l[index] != '2';
				   });

	  auto pixel = (iter != layers.end() ? (*iter)[index] : ' ');
	  pixel = (pixel == '1' ? '*' : ' ');
	  std::cout << pixel;

	  // auto pixel = '2';
	  // auto iter = layers.begin();
	  // while(pixel == '2' && iter != layers.end ())
	  //   {
	  //     pixel = (*iter)[index];
	  //     ++iter;
	  //   }

	  // pixel = (pixel == '1' ? ' ' : '*');
	  // std::cout << pixel;

	}
      std::cout << std::endl;
    }
#endif  

  layer visible {};
  for (auto i = 0; i < visible.size(); ++i)
    {
      auto iter = std::find_if(layers.begin(), layers.end(),
			       [&](layer& l)
			       {
				 return l[i] != '2';
			       });
      visible[i] = (iter != layers.end() ? (*iter)[i] : ' ');
    }
  
  Advent::printImage(visible.begin(), visible.end(), 25, 6,
		     [] (auto c) { return c == '1' ? '*' : ' '; });


  return 0;
}
