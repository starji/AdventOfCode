#pragma once

#include <iostream>
#include <iterator>
#include <vector>

namespace Advent
{
  // for use with imbue
  //     input.imbue(std::locale(input.getloc(), new advent::ctype));
  struct ctype : public std::ctype<char>
  {
    ctype(std::size_t refs = 0)
      : std::ctype<char>(custom_table, false, refs)
    {
      std::copy(classic_table(), classic_table()+table_size, custom_table);
      // custom_table['@'] |= space;
      // custom_table[':'] |= space;
      // custom_table['#'] |= space;
      // custom_table[','] |= space;
      // custom_table['['] |= space;
      // custom_table[']'] |= space;
      // custom_table['-'] |= space;
      // custom_table['.'] |= space;
      // custom_table['>'] |= space;
      // custom_table['<'] |= space;
      // custom_table['='] |= space;
      // custom_table[')'] |= space;

      // for (auto i = 'a' ; i <= 'z'; ++i)
      // {
      //     custom_table[i] |= space;
      // }
      // for (auto i = 'A' ; i <= 'Z'; ++i)
      // {
      //     custom_table[i] |= space;
      // }
    }
        
    mask custom_table[table_size];
  };

  std::vector<std::string> readLines(std::istream& input)
  {
    std::vector<std::string> result;
    std::string line;
    while(std::getline(input, line))
      {
	result.push_back(line);
      }
    return result;
  }

  template<typename T>
  std::vector<T> read(std::istream& input)
  {
    using iter_type = std::istream_iterator<T>;
    return std::vector<T>{ iter_type{input}, iter_type{} };
  }

  template<typename InputIterator>
  struct pair_to_range
  {
    using iter_pair = std::pair<InputIterator, InputIterator>;

    pair_to_range(iter_pair p)
      : p(p)
    {}

    InputIterator begin() { return p.first; }
    InputIterator end() { return p.second; }

    iter_pair p;
  };

  template<typename InputIterator>
  pair_to_range<InputIterator> make_range(std::pair<InputIterator, InputIterator> p)
  {
    return pair_to_range<InputIterator>(p);
  }

  template<typename T>
  struct identity
  {
    const T& operator()(const T& value)
    {
      return value;
    }
  };

  template<typename InputIterator, typename Convert>
  void printImage(InputIterator begin, InputIterator end, int width, int height, Convert convert)
  {
    for(auto r = 0; r < height && begin != end; ++r)
    {
      for(auto c = 0; c < width && begin != end; ++c)
	{
	  std::cout << convert(*begin);
	  ++begin;
	}
      std::cout << std::endl;
    }
  }

  template<typename InputIterator>
  void printImage(InputIterator begin, InputIterator end, int width, int height)
  {
    printImage(begin, end, width, height, identity<decltype(*begin)> {});
  }
  
}
