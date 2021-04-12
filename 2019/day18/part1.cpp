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

using Point = std::pair<int, int>;

template <typename T>
using Field = std::map<Point, T>;

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

bool isKey(char c) { return c >= 'a' && c <= 'z'; }
bool isDoor(char c) { return c >= 'A' && c <= 'Z'; }

using MazeGraph = boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
                                        boost::no_property, boost::property<boost::edge_weight_t, int>>;
using MazeVertexId = boost::graph_traits<MazeGraph>::vertex_descriptor;

struct FieldValue
{
    char value;
    MazeVertexId vertex;
};


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


using Cache = std::map<std::pair<char, std::vector<char> >, int>;
using KeyDistances = std::map<std::pair<char, char>, int>;

void printTest(const std::vector<char>& current_keys, const std::vector<char>& remaining)
{
    std::cout << "Testing: Current=";
    for(auto c : current_keys)
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


int distanceToCollect(const KeyDistances& key_distances, Cache& cache, std::vector<char> current_keys, std::vector<char> remaining)
{
    if(remaining.empty())
    {
        return 0;
    }

    auto cache_key = std::make_pair(current_keys.back(), remaining);
    auto cache_iter = cache.find(cache_key);
    if(cache_iter != cache.end())
    {
        return cache_iter->second;
    }

    printTest(current_keys, remaining);

    auto reachable = reachableKeys(current_keys);

    int result = std::numeric_limits<int>::max();
    for(auto& key : reachable)
    {
        auto next_keys = current_keys;
        next_keys.push_back(key);

        std::vector<char> next_remaining;
        std::copy_if(remaining.begin(), remaining.end(), std::back_inserter(next_remaining),
                     [&] (auto c) { return c != key; });

        auto distance = key_distances.at({current_keys.back(), key}) + distanceToCollect(key_distances, cache, next_keys, next_remaining);
        result = std::min(result, distance);
    }

    cache[cache_key] = result;
    return result;
}


int main(int argc, char* argv[])
{
    std::ifstream input("input.txt");
    // input.imbue(std::locale(input.getloc(), new Advent::ctype));

//    80x80
    Field<FieldValue> field;
    MazeGraph maze_graph;

    Point current { 0, 0 };

    auto iter = std::istream_iterator<char>{input};
    while(iter != std::istream_iterator<char>{})
    {
        field[current] = FieldValue {*iter, boost::add_vertex(maze_graph)};

        ++current.first;
        if(current.first > 80)
        {
            current.first = 0;
            ++current.second;
        }

        ++iter;
    }

    auto other_points =  [&] (auto& point) -> std::array<Point, 4>
        {
            return { add(point, delta(Direction::North)),
                     add(point, delta(Direction::South)),
                     add(point, delta(Direction::East)),
                     add(point, delta(Direction::West)) };
        };

    for(auto& p : field)
    {
        if(p.second.value != '#')
        {
            auto to_test = other_points(p.first);
            for(auto& t : to_test)
            {
                auto& to_connect = field[t];
                if(to_connect.value != '#')
                {
                    auto result = boost::add_edge(p.second.vertex, to_connect.vertex, {1}, maze_graph);
                    if(result.second)
                    {
                        // std::cout << "Connected " << p.first.first << ',' << p.first.second
                        //           << " to " << t.first << ',' << t.second
                        //           << std::endl;
                    }
                }
            }
        }
    }

    Advent::printMap(field.begin(), field.end(),
                     [](auto& v)
                     {
                         return v.value;
                     });

    std::map<std::pair<char, char>, int> key_distances;
    std::array<char, 27> ra = { '@', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
                                'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u',
                                'v', 'w', 'x', 'y', 'z' };
    for(auto& c : ra)
    {
        auto start_iter = std::find_if(field.begin(), field.end(),
                                       [&](auto& p)
                                       {
                                           return p.second.value == c;
                                       });

        std::vector<MazeVertexId> predecessor (boost::num_vertices(maze_graph));
        std::vector<int> distances (boost::num_vertices(maze_graph));

        boost::dijkstra_shortest_paths(maze_graph,
                                       start_iter->second.vertex,
                                       // boost::predecessor_map(
                                       //     boost::make_iterator_property_map(
                                       //         predecessor.begin(),
                                       //         boost::get(boost::vertex_index, maze_graph))).
                                       boost::distance_map(
                                           boost::make_iterator_property_map(
                                               distances.begin(),
                                               boost::get(boost::vertex_index, maze_graph))));


        for(auto& p : field)
        {
            if(isKey(p.second.value))
            {
//                std::cout << "Distance of " << p.second.value << " from @: " << distances[p.second.vertex] << std::endl;
                key_distances[{c, p.second.value}] = distances[p.second.vertex];
            }
        }
    }

    for(auto& p : key_distances)
    {
        std::cout << p.first.first << "->" << p.first.second << " = " << p.second << std::endl;
    }



    std::vector<char> currentKeys { '@' };
    std::vector<char> remainingKeys { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
                                      'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u',
                                      'v', 'w', 'x', 'y', 'z' };
    Cache cache;


    auto distance = distanceToCollect(key_distances, cache, currentKeys, remainingKeys);

    std::cout << "Distance = " << distance << std::endl;    

    return 0;
}


#if 0
//
Door: @ : keys=h e q u o a x 
Door: C : keys=g 
Door: D : keys=b 
Door: E : keys=n d 
Door: F : keys=t 
Door: G : keys=i w 
Door: H : keys=m 
Door: K : keys=f 
Door: L : keys=z 
Door: M : keys=v r j 
Door: S : keys=y 
Door: T : keys=l 
Door: W : keys=k 
Door: X : keys=s 
Door: Y : keys=c 
Door: Z : keys=p 



// key search?
    std::vector<char> keys;
    for(auto& p : field)
    {
        if(isKey(p.second)) { keys.push_back(p.second); }
    }

    std::vector<char> doors;
    for(auto& p : field)
    {
        if(isDoor(p.second)) { doors.push_back(p.second); }
    }

    std::cout << "Key Count= " << keys.size() << " Keys: ";
    for(auto& k : keys)
    {
        std::cout << k << ' ';
    }
    std::cout << std::endl;
    std::cout << "Door Count= " << doors.size() << " Doors: ";
    for(auto& k : doors)
    {
        std::cout << k << ' ';
    }
    std::cout << std::endl;
    

// floodfill

    std::vector<char> candidates;
    candidates.reserve(27);
    candidates.push_back('@');
    auto candidate_iter = candidates.begin();
//    while(std::any_of(field.begin(), field.end(), [](auto p) { return p.second == '.'; }))
    {
        while(candidate_iter != candidates.end())
        {
            const auto region = *candidate_iter;

            std::cout << "Candidate = " << region << std::endl;

            bool made_change = true;
            while(made_change)
            {
                made_change = false;
        
                std::vector<Point> to_modify;
                for(auto& p : field)
                {
                    if(p.second == region)
                    {
                        auto other = other_points(p.first);
                        std::copy (other.begin(), other.end(), std::back_inserter(to_modify));
                    }
                }

                for(auto& point : to_modify)
                {
                    auto& v = field[point];
                    if(v == '.' || isKey(v))
                    {
                        made_change = true;
                        if(isKey(v))
                        {
                            locked_keys[region].push_back(v);
                        }
                        v = region;
                    }
                    else if(isDoor(v) && (
                                std::find(candidates.begin(), candidates.end(), v) == candidates.end()))
                    {
                        std::cout << "Candidate found=" << v << std::endl;
                        candidates.push_back(v);
                    }        
                }
            }
            ++candidate_iter;

            Advent::printMap(field.begin(), field.end(),
                             [](auto c) { return c; });

        }
    }


    std::map<char, std::vector<char> > locked_keys;
    // Advent::printMap(field.begin(), field.end(),
    //                  [](auto c) { return c; });

    for(auto& p : locked_keys)
    {
        std::cout << "Door: " << p.first << " : keys=";
        for(auto& v : p.second)
        {
            std::cout << v << ' ';
        }
        std::cout << std::endl;
    }


#endif

