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

#include <boost/integer/mod_inverse.hpp>
#include <boost/multiprecision/cpp_int.hpp>

using Deck = std::vector<int>;

Deck newStack(Deck deck)
{
    return Deck ( deck.rbegin(), deck.rend() );
}

Deck dealWithIncrement(Deck deck, int value)
{
    Deck result ( deck.size () );

    int offset = 0;
    for(auto c : deck)
    {
        result[offset % result.size()] = c;
        offset += value;
    }

    return result;
}

Deck cut(Deck deck, int value)
{
    auto index = value;
    if(index < 0)
    {
        index = deck.size() + index;
    }

    Deck result ( deck.begin() + index, deck.end() );
    result.insert(result.end(), deck.begin(), deck.begin() + index);

    return result;
}

using Transform = std::function<Deck (Deck)>;

std::vector<Transform> readInput(std::string filename)
{
    std::vector<Transform> result;
    std::ifstream input("input.txt");

    auto iter = std::istream_iterator<std::string>(input);
    while(iter != std::istream_iterator<std::string>())
    {
        if(*iter == "deal")
        {
            ++iter;
            if(*iter == "into")
            {
                ++iter; // "new"
                ++iter; // "stack"

                result.push_back([](Deck deck) { return newStack(deck); });
            }
            else if(*iter == "with")
            {
                ++iter; // "increment"
                ++iter; // value
                
                auto value = std::stoi(*iter);
                result.push_back([=](Deck deck) { return dealWithIncrement(deck, value); });
            }
            
        }
        else if(*iter == "cut")
        {
            ++iter;// value

            auto value = std::stoi(*iter);
            result.push_back([=](Deck deck) { return cut(deck, value); });
        }

        ++iter;
    }

    return result;
}

void test1()
{
    Deck deck { 0,1,2,3,4,5,6,7,8,9 };

    auto transformed = newStack(deck);
    
    Deck expected { 9,8,7,6,5,4,3,2,1,0 };

    if(!std::equal(transformed.begin(), transformed.end(), expected.begin()))
    {
        throw std::runtime_error("test1 failed");
    }
}


void test2()
{
    Deck deck { 0,1,2,3,4,5,6,7,8,9 };

    auto transformed = cut(deck, 3);
    
    Deck expected { 3,4,5,6,7,8,9,0,1,2 };

    if(!std::equal(transformed.begin(), transformed.end(), expected.begin()))
    {
        throw std::runtime_error("test2 failed");
    }
}

void test3()
{
    Deck deck { 0,1,2,3,4,5,6,7,8,9 };

    auto transformed = cut(deck, -4);
    
    Deck expected { 6,7,8,9,0,1,2,3,4,5 };

    if(!std::equal(transformed.begin(), transformed.end(), expected.begin()))
    {
        throw std::runtime_error("test3 failed");
    }
}

void test4()
{
    Deck deck { 0,1,2,3,4,5,6,7,8,9 };

    auto transformed = dealWithIncrement(deck, 3);
    
    Deck expected { 0,7,4,1,8,5,2,9,6,3 };

    if(!std::equal(transformed.begin(), transformed.end(), expected.begin()))
    {
        throw std::runtime_error("test4 failed");
    }
}

void test5()
{
    Deck deck { 0,1,2,3,4,5,6,7,8,9 };

    deck = dealWithIncrement(deck, 7);
    deck = newStack(deck);
    deck = newStack(deck);

    auto transformed = deck;
    
    Deck expected { 0,3,6,9,2,5,8,1,4,7 };

    if(!std::equal(transformed.begin(), transformed.end(), expected.begin()))
    {
        throw std::runtime_error("test5 failed");
    }
}

void test6()
{
    Deck deck { 0,1,2,3,4,5,6,7,8,9 };

    deck = newStack(deck);
    deck = cut(deck, -2);
    deck = dealWithIncrement(deck, 7);
    deck = cut(deck, 8);
    deck = cut(deck, -4);
    deck = dealWithIncrement(deck, 7);
    deck = cut(deck, 3);
    deck = dealWithIncrement(deck, 9);
    deck = dealWithIncrement(deck, 3);
    deck = cut(deck, -1);

    auto transformed = deck;
    
    Deck expected { 9,2,5,8,1,4,7,0,3,6 };

    if(!std::equal(transformed.begin(), transformed.end(), expected.begin()))
    {
        throw std::runtime_error("test6 failed");
    }
}

Deck doTransforms(std::vector<Transform> transforms, Deck deck)
{
    for(auto& xform : transforms)
    {
        deck = xform(deck);
    }

    return deck;
}

void part1()
{
    auto transforms = readInput("input.txt");

    Deck deck;
    for(auto i = 0; i < 10007; ++i)
    {
        deck.push_back(i);
    }

    deck = doTransforms(transforms, deck);

    auto iter = std::find(deck.begin(), deck.end(), 2019);

    std::cout << "Card 2019 @ " << std::distance(deck.begin(), iter) << std::endl;    
}

void printElementLocations(const Deck& deck)
{
    auto iter = std::find(deck.begin(), deck.end(), 0);
    std::cout << "Zero at " << std::distance(deck.begin(), iter) << std::endl;
    iter = std::find(deck.begin(), deck.end(), 1);
    std::cout << "One at " << std::distance(deck.begin(), iter) << std::endl;
    iter = std::find(deck.begin(), deck.end(), 2);
    std::cout << "Two at " << std::distance(deck.begin(), iter) << std::endl;
}

void printElementsAt(const Deck& deck)
{
    std::cout << "Element at deck[0]=" << deck[0] << std::endl;
    std::cout << "Element at deck[1]=" << deck[1] << std::endl;
    std::cout << "Element at deck[2]=" << deck[2] << std::endl;
    std::cout << "Element at deck[7389]=" << deck[7389] << std::endl;
    std::cout << "Element at deck[9077]=" << deck[9077] << std::endl;
    std::cout << "Element at deck[758]=" << deck[758] << std::endl;
}

using Int = boost::multiprecision::int256_t;
const Int MOD = 119315717514047ll;//10007;
const Int REPEAT = 101741582076661ll;
const Int ONE = 1;

struct Shuffle
{
    Int offset;
    Int increment;
};

template <typename IntType>
IntType ipow(IntType base, IntType exp)
{
    int result = 1;
    for (;;)
    {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        if (!exp)
            break;
        base *= base;
    }

    return result;
}

static inline Int mod(Int a, Int b)
{
  return (a >= 0) ? (a % b) : (b + a % b);
}


static Int gcd_extended(Int a, Int b, Int& x, Int& y)
{
  if (a == 0) {
    x = 0;
    y = 1;
    return b;
  }

  Int x1, y1;
  Int gcd = gcd_extended(b % a, a, x1, y1);
  x = y1 - (b / a) * x1;
  y = x1;
  return gcd;
}

static Int modular_inverse(Int b, Int n)
{
  Int x, y;
  Int g = gcd_extended(b, n, x, y);
  return (g != 1) ? -1 : mod(x, n);
}

static Int modular_divide(Int a, Int b, Int n)
{
  a = mod(a, n);
  Int inv = modular_inverse(b, n);
  return (inv == -1) ? -1 : (a * inv) % n;
}

Int modular_power(Int base, Int exponent, Int n)
{
  if (exponent == 0) {
    return (base == 0) ? 0 : 1;
  }

  Int bit = 1;
  Int power = mod(base, n);
  Int out = 1;
  while (bit <= exponent) {
    if (exponent & bit) {
      out = mod(out * power, n);
    }
    power = mod(power * power, n);
    bit <<= 1;
  }

  return out;
}

Shuffle newStack(Shuffle shuffle)
{
    shuffle.increment *= -1;
    shuffle.increment %= MOD;
    
    shuffle.offset += shuffle.increment;
    shuffle.offset %= MOD;

    return shuffle;
}

Shuffle cut(Shuffle shuffle, Int value)
{
    shuffle.offset += shuffle.increment * value;
    shuffle.offset %= MOD;

    return shuffle;
}

Shuffle dealWithInc(Shuffle shuffle, Int value)
{
    shuffle.increment *= boost::multiprecision::powm(value, MOD - 2, MOD);
    shuffle.increment %= MOD;

    return shuffle;
}

Shuffle readInput2(std::string filename)
{
    Shuffle shuffle { 0, 1 };
    std::ifstream input("input.txt");

    auto iter = std::istream_iterator<std::string>(input);
    while(iter != std::istream_iterator<std::string>())
    {
        if(*iter == "deal")
        {
            ++iter;
            if(*iter == "into")
            {
                ++iter; // "new"
                ++iter; // "stack"

                shuffle = newStack(shuffle);
            }
            else if(*iter == "with")
            {
                ++iter; // "increment"
                ++iter; // value
                
                Int value = std::stoi(*iter);

                shuffle = dealWithInc(shuffle, value);
            }
            
        }
        else if(*iter == "cut")
        {
            ++iter;// value

            auto value = std::stoi(*iter);
            shuffle = cut(shuffle, value);
        }

        ++iter;
    }

    return shuffle;
}

Int calculate(Shuffle shuffle, Int position)
{
    return (shuffle.offset + (shuffle.increment * position)) % MOD;
}

// void printCalc(Shuffle shuffle, std::int64_t position)
// {
//     std::cout << "Value @ " << position << " = " << calculate(shuffle, position) << std::endl;
// }

int main(int argc, char* argv[])
{
    auto shuffle = readInput2("input.txt");

//    std::cout << "Offset=" << shuffle.offset << " Inc=" << shuffle.increment << std::endl;

    // printCalc(shuffle, 0);
    // printCalc(shuffle, 1);
    // printCalc(shuffle, 2);
    // printCalc(shuffle, 2018);
    // printCalc(shuffle, 2019);
    // printCalc(shuffle, 2020);
    // printCalc(shuffle, 2021);
    // printCalc(shuffle, 3073);
    // printCalc(shuffle, 3074);
    // printCalc(shuffle, 3075);
    // printCalc(shuffle, 7389);

    // std::cout << (std::int64_t) calculate(shuffle, 3074) << std::endl;
    // std::cout << (std::int64_t) calculate(shuffle, 7389) << std::endl;

    Shuffle after;
    after.increment = boost::multiprecision::powm(shuffle.increment, REPEAT, MOD);
    after.increment %= MOD;

//    after.offset = shuffle.offset * (ONE - after.increment) * boost::multiprecision::powm(ONE - shuffle.increment, MOD - 2, MOD);
    after.offset = shuffle.offset * (ONE - after.increment) * boost::integer::mod_inverse(ONE - shuffle.increment + MOD, MOD);
//    after.offset = shuffle.offset * (1 - boost::multiprecision::powm(shuffle.increment, REPEAT, MOD)) * modular_inverse(1 - shuffle.increment, MOD);

//    after.offset = divide(powm(increment, REPEAT, MOD) - 1, increment - 1)
    after.offset %= MOD;

    auto value = calculate(after, 2020);
    std::cout << value << std::endl;


    //     Deck deck;
    // for(auto i = 0; i < 10007; ++i)
    // {
    //     deck.push_back(i);
    // }

    // auto initial_value = deck[2020];

    // int number_of_runs = 0;
    // do
    // {
    //     for(auto i = 0; i < 15; ++i)
    //     {
    //         std::cout << deck[i] << ' ';
    //     }
    //     std::cout << std::endl;

    //     deck = doTransforms(transforms, deck);
    //     number_of_runs++;
    // } while(initial_value != deck[2020]);

    // for(auto i = 0; i < 15; ++i)
    // {
    //     std::cout << deck[i] << ' ';
    // }
    // std::cout << std::endl;

    // std::cout << "Took " << number_of_runs << " to repeat to factory" << std::endl;
    // std::cout << "is_sorted = " << (std::is_sorted(deck.begin(), deck.end()) ? "true" : "false") << std::endl;


    


    // std::int64_t repeats = 101741582076661ll;

    // std::int64_t prime = 119315717514047ll;

    // std::int64_t cycle = (prime - 1) / 2;

    // std::cout << repeats << std::endl;
    // std::cout << cycle << std::endl;
    // std::cout << 2 * cycle << std::endl;
    // std::cout << prime << std::endl;

    // std::cout << repeats / cycle << std::endl;
    // std::cout << repeats % cycle << std::endl;

    return 0;
}



