#include <iostream>
#include <array>
#include <thread>
#include <mutex>
#include <cstdint>
#include <fstream>
#include <iterator>
#include <functional>

namespace Intcode
{
    using Memory = std::vector<std::int64_t>;

    using opcode = std::array<std::int64_t, 4>;
    opcode separate(std::int64_t value)
    {
        return { (value % 100),
                 ((value / 100) % 10),
                 ((value / 1000) % 10),
                 ((value / 10000) % 10) };
    }

    std::ostream& operator<<(std::ostream& out, const opcode& op)
    {
        out << op[3] << op[2] << op[1] << op[0];
        return out;
    }

    struct program_exit : public std::exception
    {
        const char* what() const noexcept override { return "Program exited"; }
    };

    struct Queue
    {
        virtual void push(std::int64_t value) = 0;
        virtual std::int64_t pop() = 0;
    };

    struct ConsoleQueue : public Queue
    {
        ConsoleQueue()
            : input(std::cin), output(std::cout)
        {}

        ConsoleQueue(std::istream& input, std::ostream& output)
            : input(input), output(output)
        {}

        void push(std::int64_t value) override
        {
            std::cout << value << std::endl;
        }

        std::int64_t pop() override
        {
            std::int64_t value;
            std::cout << "Enter value: ";
            std::cin >> value;
            return value;
        }

        std::istream& input;
        std::ostream& output;
    };

    struct ExceptionQueue : public Queue
    {
        struct waiting_for_input : public std::exception
        {
            const char* what() const noexcept override { return "Waiting on input"; }
        };

        ExceptionQueue() = default;
        ExceptionQueue(std::vector<std::int64_t> queue)
            : queue(queue)
        {}
    
        void push(std::int64_t value) override
        {
            queue.push_back(value);
        }

        std::int64_t pop() override
        {
            if(queue.empty())
            {
                throw waiting_for_input {};
            }

            std::int64_t value = queue.front();
            queue.erase(queue.begin());
            return value;
        }

        std::vector<std::int64_t> queue;
    };

    struct ThreadedBlockingQueue : public Queue
    {
        ThreadedBlockingQueue() = default;
        ThreadedBlockingQueue(std::vector<std::int64_t> queue)
            : queue(queue)
        {}

        void push(std::int64_t value) override
        {
            std::lock_guard lock(mtx);
            queue.push_back(value);
        }

        std::int64_t pop() override
        {
            using namespace std::chrono_literals;
            while(queue.empty())
            {
                std::this_thread::sleep_for(1ns);
            }

            std::lock_guard lock(mtx);
            std::int64_t value = queue.front();
            queue.erase(queue.begin());
            return value;
        }

        std::mutex mtx;
        std::vector<std::int64_t> queue;
    };

    struct FunctionalQueue : public Queue
    {
        template<typename PushFunc, typename PopFunc>
        FunctionalQueue(PushFunc push_func, PopFunc pop_func)
            : on_push(push_func), on_pop(pop_func)
        {}

        void push(std::int64_t value) override
        {
            on_push(value);
        }

        std::int64_t pop() override
        {
            return on_pop();
        }

        std::function<void (std::int64_t)> on_push;
        std::function<std::int64_t (void)> on_pop;
    };
    
    struct Context
    {
        Context(const Memory& _memory, Queue& input, Queue& output)
            : memory(_memory.size() * 10000), input(input), output(output)
        {
            std::copy(_memory.begin(), _memory.end(), memory.begin());
            op = separate(immediate(0));
        }

        std::int64_t code() { return op[0]; }
        std::int64_t ip() { return _ip; }
        void ip(std::int64_t new_ip)
        {
            _ip = new_ip;
            op = separate(immediate(0));
        }
        void advanceIP(std::int64_t offset) { ip(_ip + offset); }

        std::int64_t relativeBase() { return _relative_base; }
        void addRelative(std::int64_t offset) { _relative_base += offset; }

        std::int64_t& immediate(const std::int64_t index) { return memory.at(ip() + index); }
        std::int64_t& reference(const std::int64_t index) { return memory.at(immediate(index)); }
        std::int64_t& relative(const std::int64_t index) { return memory.at(relativeBase() + immediate(index)); }
  
        std::int64_t& param(const std::int64_t index)
        {
            switch(op.at(index))
            {
                case 0:
                    return reference(index);
                case 1:
                    return immediate(index);
                case 2:
                    return relative(index);
                default:
                    throw std::runtime_error("Unknown parameter base");
            }
        }

        void halt(bool state) { _halted = state; }
        bool halted() const { return _halted; }

        std::int64_t _ip = 0;
        std::int64_t _relative_base = 0;
        bool _halted = false;
    
        Memory memory;
        Queue& input;
        Queue& output;
        opcode op;
    };

    namespace Opcode
    {
        namespace Impl
        {
            Context& invalid(Context& c)
            {
                throw std::runtime_error("Invalid opcode");
                return c;
            }

            Context& add(Context& c)
            {
                c.param(3) = c.param(1) + c.param(2);
                c.advanceIP(4);
                return c;
            }

            Context& mult(Context& c)
            {
                c.param(3) = c.param(1) * c.param(2);
                c.advanceIP(4);
                return c;
            }
      
            Context& input(Context& c)
            {
                c.param(1) = c.input.pop();
                c.advanceIP(2);
                return c;
            }
      
            Context& output(Context& c)
            {
                c.output.push(c.param(1));
                c.advanceIP(2);
                return c;
            }
      
            Context& jumpTrue(Context& c)
            {
                if (c.param(1) != 0)
                {
                    c.ip(c.param(2));
                }
                else
                {
                    c.advanceIP(3);
                }
                return c;
            }
      
            Context& jumpFalse(Context& c)
            {
                if (c.param(1) == 0)
                {
                    c.ip(c.param(2));
                }
                else
                {
                    c.advanceIP(3);
                }
                return c;
            }
      
            Context& lessThan(Context& c)
            {
                c.param(3) = c.param(1) < c.param(2) ? 1 : 0;
                c.advanceIP(4);
                return c;
            }
      
            Context& equal(Context& c)
            {
                c.param(3) = c.param(1) == c.param(2) ? 1 : 0;
                c.advanceIP(4);
                return c;
            }

            Context& adjustRelative(Context& c)
            {
                c.addRelative(c.param(1));
                c.advanceIP(2);
                return c;
            }
        }
    }

    Context& step (Context& state)
    {
        using Func = Context& (*)(Context&);
        constexpr std::array<Func, 10> Map { Opcode::Impl::invalid,     // 0
                                             Opcode::Impl::add,         // 1 - Add
                                             Opcode::Impl::mult,        // 2 - Multiply
                                             Opcode::Impl::input,       // 3 - Console input value
                                             Opcode::Impl::output,      // 4 - Console output value
                                             Opcode::Impl::jumpTrue,    // 5 - Jump if true
                                             Opcode::Impl::jumpFalse,   // 6 - Jump if false
                                             Opcode::Impl::lessThan,    // 7 - Less Than
                                             Opcode::Impl::equal,       // 8 - Equal to
                                             Opcode::Impl::adjustRelative, // 9 - Adjust relative base
        };

        if(state.code() == 99)
        {
            state.halt(true);
            return state;
        }
        else
        {
            return Map[state.code()](state);
        }
    }

    Context& run (Context& state)
    {
        while(step(state).halted() == false) {}
        return state;
    }


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
            custom_table[','] |= space;
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

    Memory parseFile(std::string filename)
    {
        std::ifstream input(filename);
        input.imbue(std::locale(input.getloc(), new Intcode::ctype));

        return Intcode::Memory (std::istream_iterator<std::int64_t>{input},
                                std::istream_iterator<std::int64_t>{});
    }
}
