#include <iostream>
#include <regex>
#include <string>
#include <algorithm>

const std::regex REPLACE {"(.)\\1{2,}"};
const std::regex FIND {"(.)\\1"};

inline bool isValid(std::string pass)
{
  auto replaced = std::regex_replace(pass, REPLACE, "");

  return std::is_sorted(pass.begin(), pass.end()) &&
    std::regex_search(replaced, FIND);
}

int main()
{
  std::vector<std::string> valid_passwords;

  for(auto i = 172851; i <= 675869; ++i)
    {
      auto password = std::to_string(i);
      if(isValid(password))
	{
	  valid_passwords.push_back(password);
	}
    }

  std::cout << valid_passwords.size() << std::endl;
}
