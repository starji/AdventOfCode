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
#include <limits>
#include <stack>
#include <fstream>
#include <numeric>
#include <cstdio>

#include "Advent.hpp"
#include "Intcode.hpp"

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/property_map/property_map.hpp>

std::vector<std::string> readMaze(std::string filename)
{
    std::vector<std::string> raw_maze;

    std::ifstream input("input.txt");
    input.imbue(std::locale(input.getloc(), new Advent::ctype));

    auto input_iter = std::istream_iterator<std::string>(input);
    while(input_iter != std::istream_iterator<std::string>())
    {
        raw_maze.push_back(*input_iter);
        ++input_iter;
    }

    return raw_maze;
}

using Point = std::pair<int, int>;

enum class Direction : std::int64_t
{
    North,
    South,
    West,
    East,
};

Point add (Point a, Point b)
{
    return { a.first + b.first, a.second + b.second };
}

Point delta (Direction value)
{
    switch(value)
    {
        case Direction::North:
            return { 0, -1 };
        case Direction::South:
            return { 0, 1 };
        case Direction::West:
            return { -1, 0 };
        case Direction::East:
            return { 1, 0 };
        default:
            throw std::runtime_error("Invalid value");
    }
}

std::array<Point, 4> cardinalPoints(Point p)
{
    return { add(p, delta(Direction::North)),
             add(p, delta(Direction::South)),
             add(p, delta(Direction::East)),
             add(p, delta(Direction::West)) };
}

char lookup(const std::vector<std::string>& raw_maze, Point p)
{
    return raw_maze[p.second][p.first];
}

std::string findName(const std::vector<std::string>& raw_maze, Point p)
{
    if(std::isalpha(lookup(raw_maze, add(p, delta(Direction::North)))))
    {
        auto second_point = add(p, delta(Direction::North));
        auto first_point = add(second_point, delta(Direction::North));
        return { lookup(raw_maze, first_point), lookup(raw_maze, second_point) };
    }
    else if(std::isalpha(lookup(raw_maze, add(p, delta(Direction::South)))))
    {
        auto first_point = add(p, delta(Direction::South));
        auto second_point = add(first_point, delta(Direction::South));
        return { lookup(raw_maze, first_point), lookup(raw_maze, second_point) };
    }
    else if(std::isalpha(lookup(raw_maze, add(p, delta(Direction::East)))))
    {
        auto first_point = add(p, delta(Direction::East));
        auto second_point = add(first_point, delta(Direction::East));
        return { lookup(raw_maze, first_point), lookup(raw_maze, second_point) };
    }
    else if(std::isalpha(lookup(raw_maze, add(p, delta(Direction::West)))))
    {
        auto second_point = add(p, delta(Direction::West));
        auto first_point = add(second_point, delta(Direction::West));
        return { lookup(raw_maze, first_point), lookup(raw_maze, second_point) };
    }
    else
    {
        return "";
    }
}


using Graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
                                    boost::property<boost::vertex_name_t, std::string>,
                                    boost::property<boost::edge_weight_t, int> >;
using GraphVertexId = boost::graph_traits<Graph>::vertex_descriptor;


template <typename PointVertexMap>
void connectVertices(const PointVertexMap& all_vertices, Graph& graph)
{
    for(auto& v : all_vertices)
    {
        auto nearby = cardinalPoints(v.first);
        for(auto& p : nearby)
        {
            auto iter = all_vertices.find(p);
            if(iter != all_vertices.end())
            {
                boost::add_edge(v.second, iter->second, {1}, graph);
            }
        }
    }
}

template <typename NamedVertexMap>
void connectNamedVertices(const NamedVertexMap& named_vertices, Graph& graph)
{
    for(auto& p : named_vertices)
    {
        if(p.second.size() == 2)
        {
            boost::add_edge(p.second.front(), p.second.back(), {1}, graph);
        }
    }
}

template <typename LayerNamedVerticesArray>
void connectLayerNamedVertices(const LayerNamedVerticesArray& layer_named_vertices, Graph& graph)
{
    auto next = layer_named_vertices.begin();
    auto first = next;
    ++next;
    while(next != layer_named_vertices.end())
    {
        auto outer_iter = first->begin();
        auto inner_iter = next->begin();
        while(outer_iter != first->end())
        {
            boost::add_edge(outer_iter->second.first, inner_iter->second.second, {1}, graph);

            ++outer_iter;
            ++inner_iter;
        }

        ++first;
        ++next;
    }
}

bool outsidePoint(Point limits, Point p)
{
    constexpr Point origin {0, 0};

    return (std::max(p.first, origin.first) - std::min(p.first, origin.first) <= 3) ||
        (std::max(p.second, origin.second) - std::min(p.second, origin.second) <= 3) ||
        (std::max(p.first, limits.first) - std::min(p.first, limits.first) <= 3) ||
        (std::max(p.second, limits.second) - std::min(p.second, limits.second) <= 3);
}

void part1()
{
    auto raw_maze = readMaze("input.txt");

    Graph graph;

    std::map<std::string, std::vector<GraphVertexId>> named_vertices;
    std::map<Point, GraphVertexId> all_vertices;

    for(auto y = 0; y < raw_maze.size(); ++y)
    {
        for(auto x = 0; x < raw_maze[y].size(); ++x)
        {
            if(raw_maze[y][x] == '.')
            {
                auto name = findName(raw_maze, {x, y});
                auto vertex_id = boost::add_vertex({name}, graph);
                all_vertices[{x, y}] = vertex_id;
                if(!name.empty())
                {
                    named_vertices[name].push_back(vertex_id);
                }
            }
        }
    }

    connectVertices(all_vertices, graph);
    connectNamedVertices(named_vertices, graph);

    std::vector<int> distances (boost::num_vertices(graph));

    boost::dijkstra_shortest_paths(graph,
                                   named_vertices["AA"].front(),
                                   boost::distance_map(
                                       boost::make_iterator_property_map(
                                           distances.begin(),
                                           boost::get(boost::vertex_index, graph))));


    std::cout << "distance to ZZ= " << distances[named_vertices["ZZ"].front()] << std::endl;
}

int main(int argc, char* argv[])
{
    auto raw_maze = readMaze("input.txt");

    Graph graph;

    GraphVertexId start;
    GraphVertexId finish;

    const int LAYERS = 30;

    std::array<std::map<std::string, std::pair<GraphVertexId, GraphVertexId> >, LAYERS> layer_named_vertices;
    std::array<std::map<Point, GraphVertexId>, LAYERS> layer_vertices;

    for(auto y = 0; y < raw_maze.size(); ++y)
    {
        for(auto x = 0; x < raw_maze[y].size(); ++x)
        {
            if(raw_maze[y][x] == '.')
            {
                auto name = findName(raw_maze, {x, y});

                for(auto i = 0; i < LAYERS; ++i)
                {
                    auto& layer = layer_vertices[i];
                    auto& layer_named = layer_named_vertices[i];

                    auto vertex_id = boost::add_vertex({name}, graph);
                    layer[{x, y}] = vertex_id;
                    if(!name.empty())
                    {
                        if(name == "AA")
                        {
                            if(i == 0)
                            {
                                start = vertex_id;
                            }
                        }
                        else if(name == "ZZ")
                        {
                            if(i == 0)
                            {
                                finish = vertex_id;
                            }
                        }
                        else
                        {
                            if(outsidePoint({raw_maze[y].size(), raw_maze.size()}, {x, y}))
                            {
                                layer_named[name].second = vertex_id;
                            }
                            else
                            {
                                layer_named[name].first = vertex_id;
                            }
                        }
                    }
                }
            }
        }
    }

    for(auto& layer : layer_vertices)
    {
        connectVertices(layer, graph);
    }
    connectLayerNamedVertices(layer_named_vertices, graph);

    std::vector<int> distances (boost::num_vertices(graph));

    boost::dijkstra_shortest_paths(graph,
                                   start,
                                   boost::distance_map(
                                       boost::make_iterator_property_map(
                                           distances.begin(),
                                           boost::get(boost::vertex_index, graph))));


    std::cout << "distance to ZZ= " << distances[finish] << std::endl;


    return 0;
}
