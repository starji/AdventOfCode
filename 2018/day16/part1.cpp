#include <iostream>
#include <fstream>
#include <vector>
#include <limits>
#include <functional>
#include <algorithm>
#include <map>
#include <iterator>

#include "advent.hpp"
#include "operations.hpp"

using Op = std::function<std::vector<std::size_t> (std::vector<std::size_t>, std::size_t, std::size_t, std::size_t)>;
using Registers = std::vector<std::size_t>;

int main (int argc, char* argv[])
{
    // muli = 6
    // addi = 0
    // addr = 4
    // borr = 2
    // mulr = 15
    // bori = 12
    // seti = 5
    // gtri = 3
    // eqir = 14
    // eqrr = 1
    // eqri = 13
    // gtrr = 9
    // gtir = 11
    // setr = 10
    // bani = 7
    // banr = 8

    std::vector<std::size_t> ignore_codes { 6, 0, 4, 2, 15, 12, 5, 3, 14, 1, 13, 9, 11, 10, 7 };

//    std::ifstream input (argv[1]);
    auto& input = std::cin;
    input.imbue(std::locale(input.getloc(), new advent::ctype));

    std::map<std::string, Op> operations {// { "addr", op::addr },
//                                           { "addi", op::addi },
//                                           { "mulr", op::mulr },
//                                           { "muli", op::muli },
                                           { "banr", op::banr },
//                                           { "bani", op::bani },
//                                           { "borr", op::borr },
//                                           { "bori", op::bori },
//                                           { "setr", op::setr },
//                                           { "seti", op::seti },
//                                           { "gtir", op::gtir },
//                                           { "gtri", op::gtri },
//                                           { "gtrr", op::gtrr },
//                                           { "eqir", op::eqir },
//                                           { "eqri", op::eqri },
//                                           { "eqrr", op::eqrr }
    };
    // std::vector<Op> operations { op::addr, op::addi, op::mulr, op::muli, op::banr, op::bani,
    //                              op::borr, op::bori, op::setr, op::seti,
    //                              op::gtir, op::gtii, op::gtrr, op::eqir, op::eqri, op::eqrr };

    std::size_t matching_values = 0;
    do
    {
        Registers registers { std::numeric_limits<std::size_t>::max (),
                              std::numeric_limits<std::size_t>::max (),
                              std::numeric_limits<std::size_t>::max (),
                              std::numeric_limits<std::size_t>::max () };
        
        Registers expected { std::numeric_limits<std::size_t>::max (),
                             std::numeric_limits<std::size_t>::max (),
                             std::numeric_limits<std::size_t>::max (),
                             std::numeric_limits<std::size_t>::max () };
        std::size_t opcode = std::numeric_limits<std::size_t>::max ();
        std::size_t a = std::numeric_limits<std::size_t>::max ();
        std::size_t b = std::numeric_limits<std::size_t>::max ();
        std::size_t c = std::numeric_limits<std::size_t>::max ();

        input >> registers[0] >> registers[1] >> registers[2] >> registers[3];
        input >> opcode >> a >> b >> c;
        input >> expected[0] >> expected[1] >> expected[2] >> expected[3];

        if (std::count (ignore_codes.begin (), ignore_codes.end (), opcode) == 0)
        {

            try
            {
                std::map<std::string, Registers> op_results;
                std::transform (operations.begin (), operations.end (),
                                std::inserter (op_results, op_results.begin ()),
                                [&] (std::pair<const std::string, Op>& op_pair) -> std::pair<std::string, Registers>
                                {
                                    return std::make_pair (op_pair.first, op_pair.second (registers, a, b, c));
                                });
            
                auto result_count = std::count_if(op_results.begin (), op_results.end (),
                                                  [&](const std::pair<const std::string, Registers>& c)
                                                  {
                                                      return c.second == expected;
                                                  });
                if (result_count == 1)
                {
                    std::cout << '[';
                    for (auto& c : registers)
                    {
                        std::cout << c << ' ';
                    }
                    std::cout << ']' << std::endl;

                    std::cout << '(' << opcode << ' ' << a << ' ' << b << ' ' << c << ')' << std::endl;

                    std::cout << '*';
                    for (auto& c : expected)
                    {
                        std::cout << c << ' ';
                    }
                    std::cout << '*' << std::endl;

                    for (auto& r : op_results)
                    {
                        std::cout << r.first << "=";
                        for (auto& c : r.second)
                        {
                            std::cout << c << ' ';
                        }
                        std::cout << std::endl;
                    }

                    std::cout << "Result count=" << result_count << std::endl;
                    ++matching_values;
                }
            }
            catch (std::exception& e)
            {
                // ignore?
            }
        }
            
        input.ignore (std::numeric_limits<std::streamsize>::max(), '\n');
    } while (input.good());

    std::cout << "3 or more: " << matching_values << std::endl;    

    return 0;
}
