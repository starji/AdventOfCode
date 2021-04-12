#include <iostream>
#include <array>
#include <algorithm>
#include <vector>

inline bool sixdigits(int password)
{
  return password >= 172851 || password <= 675869;
}

using password = std::array<int, 6>;

password separate(int pass)
{
  password result;

  auto iter = result.rbegin();
  while(pass > 0)
    {
      auto val = pass % 10;
      *iter = val;
      ++iter;
      pass /= 10;
    }

  return result;
}

inline bool isValid(password pass)
{
  return std::is_sorted(pass.begin(), pass.end()) &&
    std::adjacent_find(pass.begin(), pass.end()) != pass.end();
}

inline bool isValid2(password pass)
{
  bool found = false;
  auto iter = std::adjacent_find(pass.begin(), pass.end());
  while(iter != pass.end())
    {
      auto num = std::count(iter, pass.end(), *iter);
      if (num == 2)
	{
	  found = true;
	}
      iter = std::adjacent_find(iter + num, pass.end());
    }
  return found;
}

void test(int pass)
{
  std::cout << "Testing " << pass << " : " << (isValid2(separate(pass)) ? "True" : "False") << std::endl;
}

int main()
{
  std::vector<int> possible;

  for(auto i = 172851; i <= 675869; ++i)
    {
      if(isValid(separate(i)))
	{
	  possible.push_back(i);
	}
    }

  std::cout << "Count=" << possible.size() << std::endl;

  std::vector<int> possible2;

  std::copy_if(possible.begin(), possible.end(), std::back_inserter(possible2),
	       [] (int val) { return isValid2(separate(val)); });


  std::cout << "Count2 = " << possible2.size() << std::endl;

  return 0;
}
