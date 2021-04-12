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
#include <list>

#include "Advent.hpp"
#include "Intcode.hpp"

#include <boost/graph/undirected_graph.hpp>
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

bool isKey(char c) { return c >= 'a' && c <= 'z'; }
bool isDoor(char c) { return c >= 'A' && c <= 'Z'; }

using Graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
                                    boost::property<boost::vertex_name_t, char>,
                                    boost::property<boost::edge_weight_t, int> >;
using GraphVertexId = boost::graph_traits<Graph>::vertex_descriptor;

std::vector<char> reachableKeys(std::vector<char> current_keys)
{
    // lol
    std::map<char, std::vector<char> > locked_behind;
    locked_behind['@'] = { 'h', 'e', 'q', 'u', 'o', 'a', 'x' }; 
    locked_behind['c'] = { 'g' }; 
    locked_behind['d'] = { 'b' }; 
    locked_behind['e'] = { 'n', 'd' }; 
    locked_behind['f'] = { 't' }; 
    locked_behind['g'] = { 'i', 'w' }; 
    locked_behind['h'] = { 'm' }; 
    locked_behind['k'] = { 'f' }; 
    locked_behind['l'] = { 'z' }; 
    locked_behind['m'] = { 'v', 'r', 'j' }; 
    locked_behind['s'] = { 'y' }; 
    locked_behind['t'] = { 'l' }; 
    locked_behind['w'] = { 'k' }; 
    locked_behind['x'] = { 's' }; 
    locked_behind['y'] = { 'c' }; 
    locked_behind['z'] = { 'p' }; 

    std::sort(current_keys.begin(), current_keys.end());

    std::vector<char> possible;
    for(auto& k : current_keys)
    {
        auto iter = locked_behind.find(k);
        if(iter != locked_behind.end())
        {
            std::copy(iter->second.begin(), iter->second.end(), std::back_inserter(possible));
        }
    }

    std::sort(possible.begin(), possible.end());

    std::vector<char> result;

    std::set_difference(possible.begin(), possible.end(),
                        current_keys.begin(), current_keys.end(),
                        std::back_inserter(result));

    return result;
}

using NamedLocks = std::map<char, std::vector<char> >;
using VertexLocks = std::map<GraphVertexId, std::vector<GraphVertexId> >;

NamedLocks getNamedLocks()
{
    std::map<char, std::vector<char> > locked_behind;
    locked_behind['c'] = { 'g' }; 
    locked_behind['d'] = { 'b' }; 
    locked_behind['e'] = { 'n', 'd' }; 
    locked_behind['f'] = { 't' }; 
    locked_behind['g'] = { 'i', 'w' }; 
    locked_behind['h'] = { 'm' }; 
    locked_behind['k'] = { 'f' }; 
    locked_behind['l'] = { 'z' }; 
    locked_behind['m'] = { 'v', 'r', 'j' }; 
    locked_behind['s'] = { 'y' }; 
    locked_behind['t'] = { 'l' }; 
    locked_behind['w'] = { 'k' }; 
    locked_behind['x'] = { 's' }; 
    locked_behind['y'] = { 'c' }; 
    locked_behind['z'] = { 'p' };
    return locked_behind;
}

VertexLocks getVertexLocks(std::map<char, GraphVertexId> name_vertex_map, NamedLocks named_locks)
{
    VertexLocks vertex_locks;

    for(auto& p : named_locks)
    {
        auto& behind_lock = p.second;

        std::vector<GraphVertexId> behind_vertices;
        std::transform(behind_lock.begin(), behind_lock.end(),
                       std::back_inserter(behind_vertices),
                       [&](auto c) { return name_vertex_map.at(c); });

        vertex_locks.insert({ name_vertex_map.at(p.first), behind_vertices });
    }

    return vertex_locks;
}

std::vector<GraphVertexId> reachableKeys(const VertexLocks& locks, std::vector<GraphVertexId> remaining)
{
    std::vector<GraphVertexId> locked;
    for(auto& v : remaining)
    {
        auto iter = locks.find(v);
        if(iter != locks.end())
        {
            locked.insert(locked.end(), iter->second.begin(), iter->second.end());
        }
    }

    std::sort(locked.begin(), locked.end());

    std::vector<GraphVertexId> reachable;

    std::set_difference(remaining.begin(), remaining.end(),
                        locked.begin(), locked.end(),
                        std::back_inserter(reachable));

    return reachable;
}

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

using KeyDistances = std::map<std::pair<GraphVertexId, GraphVertexId>, int>;

KeyDistances calculateDistanceMap(Graph& graph, std::vector<GraphVertexId> vertices)
{
    KeyDistances distance_map;

    for(auto& id : vertices)
    {
        std::vector<int> distances (boost::num_vertices(graph));
        boost::dijkstra_shortest_paths(graph,
                                       id,
                                       boost::distance_map(
                                           boost::make_iterator_property_map(
                                               distances.begin(),
                                               boost::get(boost::vertex_index, graph))));

        for(auto& v : vertices)
        {
            distance_map[{id, v}] = distances[v];
        }
    }

    return distance_map;
}

using VertexState = std::array<GraphVertexId, 4>;
using Cache = std::map<std::pair<VertexState, std::vector<GraphVertexId> >, int>;

void printDistanceCollect(const VertexState& current_state, const std::vector<GraphVertexId>& remaining)
{
    std::cout << "Testing: Current=";
    for(auto c : current_state)
    {
        std::cout << c << ' ';
    }
    std::cout << "Remaining=";
    for(auto c : remaining)
    {
        std::cout << c << ' ';
    }
    std::cout << std::endl;
}


int distanceToCollect(const KeyDistances& distance_map, const VertexLocks& locks, Cache& cache, VertexState current_state, std::vector<GraphVertexId> remaining)
{
    if(remaining.empty())
    {
        return 0;
    }

    auto cache_key = std::make_pair(current_state, remaining);
    auto cache_iter = cache.find(cache_key);
    if(cache_iter != cache.end())
    {
        return cache_iter->second;
    }

    int result = std::numeric_limits<int>::max();

    auto reachable_keys = reachableKeys(locks, remaining);
    for(auto& target : reachable_keys)
    {
        int distance = std::numeric_limits<int>::max();
        int index = 0;
        while(distance == std::numeric_limits<int>::max() && index < 4)
        {
            distance = distance_map.at({current_state[index], target});
            if(distance == std::numeric_limits<int>::max())
            {
                ++index;
            }
        }

        auto copy_state = current_state;
        copy_state[index] = target;
        std::sort(copy_state.begin(), copy_state.end());

        std::vector<GraphVertexId> next_remaining;
        std::copy_if(remaining.begin(), remaining.end(), std::back_inserter(next_remaining),
                     [&](auto v) { return v != target; });
            
        distance += distanceToCollect(distance_map, locks, cache, copy_state, next_remaining);
        result = std::min(result, distance);
    }

    cache[cache_key] = result;    
    return result;
}

int main(int argc, char* argv[])
{
    auto raw = readMaze("input.txt");

    Graph graph;

    std::map<Point, GraphVertexId> all_vertices;
    std::map<char, GraphVertexId> named_keys;
    std::vector<GraphVertexId> keys;
    std::vector<GraphVertexId> start_vertices;

    for(auto y = 0; y < raw.size(); ++y)
    {
        for(auto x = 0; x < raw[y].size(); ++x)
        {
            auto name = raw[y][x];
            if(name != '#')
            {
                auto vertex_id = boost::add_vertex({name}, graph);
                all_vertices[{x, y}] = vertex_id;
                if(isKey(name))
                {
                    named_keys[name] = vertex_id;
                    keys.push_back(vertex_id);
                }
                if(name == '@')
                {
                    start_vertices.push_back(vertex_id);
                }
            }
        }
    }

    std::sort(start_vertices.begin(), start_vertices.end());

    connectVertices(all_vertices, graph);

    std::vector<GraphVertexId> distance_vertices(start_vertices.begin(), start_vertices.end());
    std::copy(keys.begin(), keys.end(), std::back_inserter(distance_vertices));
    auto distance_map = calculateDistanceMap(graph, distance_vertices);
    auto locks = getVertexLocks(named_keys, getNamedLocks ());

    Cache cache;

    std::array<GraphVertexId, 4> vertex_state { start_vertices[0], start_vertices[1],
                                                start_vertices[2], start_vertices[3] };

    auto distance = distanceToCollect(distance_map, locks, cache, vertex_state, keys);


    std::cout << "Distance = " << distance << std::endl;


    return 0;
}
