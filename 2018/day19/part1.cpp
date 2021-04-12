#include <iostream>
#include <functional>
#include <limits>
#include <map>

#include "advent.hpp"
#include "operations.hpp"

using Registers = std::vector<std::size_t>;
using Op = std::function<Registers (Registers, std::size_t, std::size_t, std::size_t)>;

struct Instruction
{
    Instruction(std::string name, Op operation, std::size_t a, std::size_t b, std::size_t c)
        : name (name), operation (operation), a(a), b(b), c(c)
    {}

    Registers operator()(Registers registers)
    {
        return operation (registers, a, b, c);
    }

    std::string name;
    Op operation;
    std::size_t a, b, c;
};

struct Machine
{
    Machine ()
        : ip(5), registers { 1, 0, 0, 0, 0, 0 }
    {}

    void operator()()
    {
//        print_status ();
        auto& instruction_index = registers[ip];
        if (instruction_index >= memory.size ())
        {
            std::cout << "ii=" << instruction_index << " size=" << memory.size () << std::endl;
            throw std::out_of_range ("Done");
        }
        auto& instruction = memory[registers[ip]];
        registers = instruction (registers);

        // std::cout << ' ' << instruction.name
        //           << ' ' << instruction.a
        //           << ' ' << instruction.b
        //           << ' ' << instruction.c
        //           << ' ';
        // print_status ();
        // std::cout << std::endl;

        registers[ip]++;
    }

    void print_status ()
    {
        std::cout << "ip=" << registers[ip] << " ";
        std::cout << '[';
        for (auto& r : registers)
        {
            std::cout << r << ' ';
        }
        std::cout << ']';
        auto& instruction = memory[registers[ip]];
        std::cout << ' ' << instruction.name
                  << ' ' << instruction.a
                  << ' ' << instruction.b
                  << ' ' << instruction.c
                  << ' ';
    }

    std::size_t ip = 5;
    Registers registers;
    std::vector<Instruction> memory;
};


int main ()
{
    auto& input = std::cin;
    input.imbue(std::locale(input.getloc(), new advent::ctype));

    std::map<std::string, Op> ops { { "addr", op::addr },
                                    { "addi", op::addi },
                                    { "mulr", op::mulr },
                                    { "muli", op::muli },
                                    { "banr", op::banr },
                                    { "bani", op::bani },
                                    { "borr", op::borr },
                                    { "bori", op::bori },
                                    { "setr", op::setr },
                                    { "seti", op::seti },
                                    { "gtir", op::gtir },
                                    { "gtri", op::gtri },
                                    { "gtrr", op::gtrr },
                                    { "eqir", op::eqir },
                                    { "eqri", op::eqri },
                                    { "eqrr", op::eqrr } };

    Machine machine;

    do
    {
        std::string opcode;
        std::size_t a = std::numeric_limits<std::size_t>::max ();
        std::size_t b = std::numeric_limits<std::size_t>::max ();
        std::size_t c = std::numeric_limits<std::size_t>::max ();
        input >> opcode >> a >> b >> c;

        try
        {
            Op func = ops.at(opcode);
            machine.memory.push_back (Instruction { opcode, func, a, b, c });
        }
        catch (std::exception& e)
        {
            std::cout << "failed opcode=" << opcode << std::endl;
        }

        input.ignore (std::numeric_limits<std::streamsize>::max(), '\n');
    } while (input.good());

    std::uint64_t count = 0;
    try
    {
        while (++count > 0)
        {
            machine ();
//            if (count % 1000000 == 0)
            {
                std::cout << count << ' ';
                machine.print_status ();
                std::cout << std::endl;
            }
        }
    }
    catch (std::out_of_range& e)
    {
        std::cout << e.what () << " " << machine.registers[0] << " steps=" << count << std::endl;
    }


    return 0;
}

    
