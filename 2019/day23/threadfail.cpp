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

struct CommonBus
{
    CommonBus();

    void run();

    void push(Packet packet);

    std::mutex mut;
    std::atomic<bool> exit_thread = false;
    std::queue<Packet> queue;
    std::vector<class NetworkedQueue> machines;
};

struct NetworkedQueue : public Intcode::Queue
{
    enum class PushState
    {
        Address,
        X,
        Y
    };

    NetworkedQueue(std::int64_t address, CommonBus& common_bus)
        : address(address), common_bus(common_bus), mut(new std::mutex)
    {
        queue.push(address);
    }

    void push(std::int64_t value) override
    {
        switch(state)
        {
            case PushState::Address:
                std::get<0>(pending) = value;
                state = PushState::Address;
                break;
            case PushState::X:
                std::get<1>(pending) = value;
                state = PushState::Address;
                break;
            case PushState::Y:
                std::get<2>(pending) = value;
                common_bus.push(pending);
                state = PushState::Address;
                break;
        }
    }

    std::int64_t pop() override
    {
        using namespace std::chrono_literals;
        std::lock_guard lock(*mut);
        std::int64_t value = -1;
        if(queue.empty())
        {
            std::this_thread::sleep_for(1ns);
        }
        else
        {
            std::int64_t value = queue.front();
            queue.pop();
        }
        return value;
    }

    void push(std::int64_t x, std::int64_t y)
    {
        std::lock_guard lock(*mut);
        queue.push(x);
        queue.push(y);
    }

    std::int64_t address;
    CommonBus& common_bus;

    PushState state = PushState::Address;
    Packet pending;

    std::shared_ptr<std::mutex> mut;
    std::queue<std::int64_t> queue;
};

CommonBus::CommonBus()
{
    machines.reserve(50);
}

void CommonBus::run()
{
    using namespace std::chrono_literals;

    while(exit_thread == false)
    {
        {
            std::lock_guard lock(mut);
            if(!queue.empty())
            {
                auto& address = std::get<0>(queue.front());
                auto& x = std::get<1>(queue.front());
                auto& y = std::get<2>(queue.front());

                if(address == 255)
                {
                    std::cout << "255 X=" << x << " y=" << y << std::endl;
                    exit_thread = true;
                }
                else
                {
                    machines[address].push(x, y);
                }

                queue.pop();
            }
        }

        std::this_thread::sleep_for(1ns);
    }
}

void CommonBus::push(Packet packet)
{
    std::lock_guard lock(mut);
    std::cout << "Common bus receiving: "
              << std::get<0>(packet) << " "
              << std::get<1>(packet) << " "
              << std::get<2>(packet) << " "
              << std::endl;
    queue.push(packet);
}

int main(int argc, char* argv[])
{
    using namespace std::chrono_literals;

    auto memory = Intcode::parseFile("input.txt");

    std::set_terminate([]() { std::cout << "Terminate Called"; });

    CommonBus common_bus;

    std::vector<std::thread> machines;
    machines.reserve(50);

    std::atomic<bool> go = false;

    for(auto i = 0; i < 50; ++i)
    {

        common_bus.machines.push_back(NetworkedQueue(i, common_bus));
        auto& last_queue = common_bus.machines.back();
        Intcode::Context ctx { memory, last_queue, last_queue };

        machines.push_back(std::thread(
                               [&go, ctx] () mutable
                               {
                                   while(!go)
                                   {
                                       std::this_thread::sleep_for(1ns);
                                   }
                                   Intcode::run(ctx);
                               }));
    }

    go = true;
    common_bus.run();

    for(auto& m : machines)
    {
        m.join();
    }

    return 0;
}



