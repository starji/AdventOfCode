#include <iostream>
#include <locale>
#include <map>
#include <string>

#include <boost/graph/adjacency_list.hpp>

#include "Advent.hpp"

namespace std
{
  template<typename T>
  istream& operator>>(istream& input, pair<T, T>& value)
  {
    input >> value.first;
    input >> value.second;
    return input;
  }
}

int main(int argc, char* argv[])
{
  auto& input = std::cin;
  input.imbue(std::locale(input.getloc(), new Advent::ctype));

  auto values = Advent::read<std::pair<std::string,std::string>>(input);


  //  using VertexProperty = boost::property<boost::vertex_name_t, std::string>;
  using Graph = boost::adjacency_list<boost::vecS,
				      boost::vecS,
				      boost::undirectedS,
				      std::string, std::string
				      >;

  Graph graph;

  boost::add_vertex(std::string("A"), graph);
  boost::add_vertex(std::string("B"), graph);
  boost::add_vertex(std::string("C"), graph);

  //  auto props = boost::get(boost::vertex_name_t{}, graph)

  boost::add_edge(0, 1, graph);
  boost::add_edge(1, 2, graph);

  auto [iter, end] = boost::vertices(graph);
  while(iter != end)
    {
      std::cout << graph[*iter] << std::endl;
      ++iter;
    }

  

}
