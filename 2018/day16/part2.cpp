#include <iostream>
#include <functional>
#include <limits>

#include "advent.hpp"
#include "operations.hpp"

    // addi = 0
    // eqrr = 1
    // borr = 2
    // gtri = 3
    // addr = 4
    // seti = 5
    // muli = 6
    // bani = 7
    // banr = 8
    // gtrr = 9
    // setr = 10
    // gtir = 11
    // bori = 12
    // eqri = 13
    // eqir = 14
    // mulr = 15

using Registers = std::vector<std::size_t>;
using Op = std::function<Registers (Registers, std::size_t, std::size_t, std::size_t)>;

struct Machine
{
    Machine ()
        : registers { 0, 0, 0, 0 },
          ops {
              op::addi, op::eqrr, op::borr, op::gtri, op::addr, op::seti, op::muli, op::bani,
              op::banr, op::gtrr, op::setr, op::gtir, op::bori, op::eqri, op::eqir, op::mulr
          }
    {}

    void operator()(std::size_t op, std::size_t a, std::size_t b, std::size_t c)
    {
        registers = ops[op](registers, a, b, c);
    }

    Registers registers;
    std::vector<Op> ops;
};

int main ()
{
    auto& input = std::cin;
    input.imbue(std::locale(input.getloc(), new advent::ctype));

    Machine machine;

    do
    {
        std::size_t opcode = std::numeric_limits<std::size_t>::max ();
        std::size_t a = std::numeric_limits<std::size_t>::max ();
        std::size_t b = std::numeric_limits<std::size_t>::max ();
        std::size_t c = std::numeric_limits<std::size_t>::max ();
        input >> opcode >> a >> b >> c;

        if (opcode != std::numeric_limits<std::size_t>::max ())
        {
            machine(opcode, a, b, c);
        }

        input.ignore (std::numeric_limits<std::streamsize>::max(), '\n');
    } while (input.good());

    std::cout << machine.registers[0] << std::endl;

    return 0;
}

    
