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
#include <queue>
#include <atomic>

#include "Advent.hpp"
#include "Intcode.hpp"

using Packet = std::tuple<std::int64_t, std::int64_t, std::int64_t>;

struct OutputQueue : public Intcode::Queue
{
    void push(std::int64_t value) override
    {
        queue.push(value);
    }

    std::int64_t pop() override
    {
        if(queue.empty())
        {
            return -1;
        }
        else
        {
            std::int64_t value = queue.front();
            queue.pop();
            return value;
        }        
    }

    std::queue<std::int64_t> queue;
};

struct NAT
{
    NAT(std::vector<OutputQueue>& queues)
        : queues(queues)
    {}

    void check()
    {
        if(count < 10000)
        {
            if (std::all_of(queues.begin(), queues.end(),
                            [](auto& q) { return q.queue.empty(); }))
            {                
                count++;
            }
            else
            {
                count = 0;
            }
        }
        else
        {
            if (std::all_of(queues.begin(), queues.end(),
                            [](auto& q) { return q.queue.empty(); }))
            {
                if(lastX == x && lastY == y)
                {
                    std::cout << "EXIT CONDITION REACHED: X=" << x << " Y=" << y << std::endl;
                    done = true;
                }

                lastX = x;
                lastY = y;

                queues[0].push(x);
                queues[0].push(y);
                trigger_count++;
            }            
            count = 0;
        }
    }

    bool done = false;
    int count = 0;

    int trigger_count = 0;

    std::int64_t lastX = 0, lastY = 0;
    std::int64_t x = 0, y = 0;
    std::vector<OutputQueue>& queues;    
};

struct BusProxy : public Intcode::Queue
{
    enum class PushState
    {
        Address,
        X,
        Y
    };

    BusProxy(std::vector<OutputQueue>& output_queues, NAT& nat)
        : output_queues(output_queues), nat(nat)
    {}

    void push(std::int64_t value) override
    {
        switch(state)
        {
            case PushState::Address:
                address = value;
                state = PushState::X;
                break;
            case PushState::X:
                x = value;
                state = PushState::Y;
                break;
            case PushState::Y:
                y = value;
                state = PushState::Address;

                if(address == 255)
                {
                    std::cout << "Nat getting " << x << ',' << y << std::endl;
                    nat.x = x;
                    nat.y = y;
                }
                else
                {
                    output_queues[address].push(x);
                    output_queues[address].push(y);
                }
                break;
        }
    }

    std::int64_t pop() override { throw std::runtime_error("Not yet implemented"); }

    PushState state = PushState::Address;
    std::int64_t address = 0, x = 0, y = 0;
    std::vector<OutputQueue>& output_queues;
    NAT& nat;
};


int main(int argc, char* argv[])
{
    using namespace std::chrono_literals;

    // Intcode::Memory memory { 3,60,1005,60,18,1101,0,1,61,4,61,104,1011,104,1,1105,1,22,1101,0,0,61,3,62,1007,62,0,64,1005,64,22,3,63,1002,63,2,63,1007,63,256,65,1005,65,48,1101,0,255,61,4,61,4,62,4,63,1105,1,22,99 };

    auto memory = Intcode::parseFile("input.txt");

    std::vector<OutputQueue> queues;
    std::vector<BusProxy> proxies;
    std::vector<Intcode::Context> contexts;
    NAT nat (queues);

    queues.reserve(53);
    proxies.reserve(53);
    contexts.reserve(53);

    for(auto i = 0; i < 50; i++)
    {
        queues.push_back(OutputQueue ());
        queues.back().push(i);
        
        proxies.push_back(BusProxy (queues, nat));
        
        contexts.emplace_back(memory, queues.back(), proxies.back());
    }

    int index = 0;
    while(!nat.done)
    {
        // for(auto& q : queues)
        // {
        //     std::cout << (q.queue.empty() ? '.' : '*');
        // }
        // std::cout << std::endl;

        for(auto& c : contexts)
        {
            Intcode::step(c);
        }

        nat.check();
    }

    return 0;
}



