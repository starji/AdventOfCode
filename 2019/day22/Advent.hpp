#pragma once

#include <iostream>
#include <iterator>
#include <vector>

namespace Advent
{
    // Standard typedefs

    // typically x = horiz, y = vert
    //  +x = right, -x = left, +y = Down, -y = up
    using Point = std::pair<int, int>;


    // for use with imbue
    //     input.imbue(std::locale(input.getloc(), new advent::ctype));
    struct ctype : public std::ctype<char>
    {
        ctype(std::size_t refs = 0)
            : std::ctype<char>(custom_table, false, refs)
        {
            std::copy(classic_table(), classic_table()+table_size, custom_table);
            //custom_table[' '] &= ~space;

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

    template<typename InputIterator, typename Convert>
    void printMap (InputIterator begin, InputIterator end, Convert convert)
    {
        auto compare_x = [] (auto a, auto b) { return a.first.first < b.first.first; };
        auto compare_y = [] (auto a, auto b) { return a.first.second < b.first.second; };
        auto leftmost = min_element(begin, end, compare_x)->first.first;
        auto rightmost = max_element(begin, end, compare_x)->first.first;
        auto topmost = min_element(begin, end, compare_y)->first.second;
        auto bottommost = max_element(begin, end, compare_y)->first.second;

        // std::cout << "Left=" << leftmost
        // 	      << "Right=" << rightmost
        // 	      << "Top=" << topmost
        // 	      << "Bottom=" << bottommost
        // 	      << std::endl;

        auto width = (rightmost - leftmost) + 1;
        auto height = (bottommost - topmost) + 1;
        // std::cout << "Width=" << width
        // 	      << "Height=" << height
        // 	      << std::endl;

        std::vector<char> values (width*height, ' ' );
        while(begin != end)
        {
            auto x = begin->first.first - leftmost;
            auto y = (begin->first.second - topmost) * width;
            values[x + y] = convert(begin->second);
            ++begin;
        }
        printImage(values.begin(), values.end(), width, height);
    }

    // Print an image to the console from iterators composed of points.
    template<typename InputIterator>
    void printPoints(InputIterator begin, InputIterator end)
    {
        auto compare_x = [] (auto a, auto b) { return a.first < b.first; };
        auto compare_y = [] (auto a, auto b) { return a.second < b.second; };
        auto leftmost = min_element(begin, end, compare_x)->first;
        auto rightmost = max_element(begin, end, compare_x)->first;
        auto topmost = min_element(begin, end, compare_y)->second;
        auto bottommost = max_element(begin, end, compare_y)->second;

        // std::cout << "Left=" << leftmost
        // 	      << "Right=" << rightmost
        // 	      << "Top=" << topmost
        // 	      << "Bottom=" << bottommost
        // 	      << std::endl;

        auto width = (rightmost - leftmost) + 1;
        auto height = (bottommost - topmost) + 1;

        // std::cout << "Width=" << width
        // 	      << "Height=" << height
        // 	      << std::endl;
        std::vector<char> values (width*height, ' ' );
        while(begin != end)
        {
            auto x = begin->first - leftmost;
            auto y = (begin->second - topmost) * width;

            //std::cout << begin->first << ',' << begin->second << " " << (x + y) << std::endl;
            values[x + y] = '#';
            ++begin;
        }
        printImage(values.begin(), values.end(), width, height);
    }  
}
