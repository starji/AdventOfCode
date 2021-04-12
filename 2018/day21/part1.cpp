#include <iostream>
#include <array>
#include <map>

using Registers = std::array</*std::size_t*/ int, 6>;

void print_registers (const Registers& registers)
{
    std::cout << '[' << registers[0]
              << ' ' << registers[1]
              << ' ' << registers[2]
              << ' ' << registers[3]
              << ' ' << registers[4]
              << ' ' << registers[5]
              << ']';
}

int main()
{
    const std::size_t ip = 5;
    std::array<std::string, 36> names {
        "seti 123 0 3",
        "bani 3 456 3",
        "eqri 3 72 3",
        "addr 3 5 5",
        "seti 0 0 5",
        "seti 0 5 3",
        "bori 3 65536 2",
        "seti 832312 1 3",
        "bani 2 255 1",
        "addr 3 1 3",
        "bani 3 16777215 3",
        "muli 3 65899 3",
        "bani 3 16777215 3",
        "gtir 256 2 1",
        "addr 1 5 5",
        "addi 5 1 5",
        "seti 27 7 5",
        "seti 0 2 1",
        "addi 1 1 4",
        "muli 4 256 4",
        "gtrr 4 2 4",
        "addr 4 5 5",
        "addi 5 1 5",
        "seti 25 1 5",
        "addi 1 1 1",
        "seti 17 0 5",
        "setr 1 7 2",
        "seti 7 2 5",
        "eqrr 3 0 1",
        "addr 1 5 5",
        "seti 5 5 5",
    };
    Registers r { 0, 0, 0, 0, 0, 0 };

    std::uint64_t count = 0;
    bool finished = false;

    auto print_before = [&] ()
                        {
                            std::cout << "ip=" << r[ip] << ' ';
                            print_registers (r);
                            if (r[ip] < names.size ())
                            {
                                std::cout << ' ' << names[r[ip]] << ' ';
                            }
                        };
    auto print_after = [&] ()
                       {
                           print_registers (r);
                           std::cout << std::endl;
                       };

    std::map<int, std::size_t> endpoints;

    while (!finished)
    {

        // if (count % 1000000 == 0)
        // {
        // }

        switch (r[ip])
        {
            case 0:
                r[3] = 123; //seti 123 0 3
                break;
            case 1:
                r[3] = r[3] & 456; //bani 3 456 3
                break;
            case 2:
                r[3] = r[3] == 72 ? 1 : 0; //eqri 3 72 3
                break;
            case 3:
                r[5] = r[5] + r[3]; //addr 3 5 5
                break;
            case 4:
                r[5] = 0; //seti 0 0 5
                break;
            case 5:
                r[3] = 0; //seti 0 5 3
                break;
            case 6:
                r[2] = r[3] | 65536; //bori 3 65536 2
                break;
            case 7:
                r[3] = 832312; //seti 832312 1 3
                break;
            case 8:
                r[1] = r[2] & 255; //bani 2 255 1
                break;
            case 9:
                r[3] = r[3] + r[1]; //addr 3 1 3
                break;
            case 10:
                r[3] = r[3] & 16777215; //bani 3 16777215 3
                break;
            case 11:
                r[3] = r[3] * 65899; //muli 3 65899 3
                break;
            case 12:
                r[3] = r[3] & 16777215; //bani 3 16777215 3
                break;
            case 13:
                r[1] = 256 > r[2] ? 1 : 0; //gtir 256 2 1
                break;
            case 14:
                r[5] = r[5] + r[1]; //addr 1 5 5
                break;
            case 15:
                r[5] = r[5] + 1; //addi 5 1 5
                break;
            case 16:
                r[5] = 27; //seti 27 7 5
                break;
            case 17:
                r[1] = 0; //seti 0 2 1
                break;
            case 18:
                r[4] = r[1] + 1; //addi 1 1 4
                break;
            case 19:
                r[4] = r[4] * 256; //muli 4 256 4
                break;
            case 20:
                r[4] = r[4] > r[2] ? 1 : 0; //gtrr 4 2 4
                break;
            case 21:
                r[5] = r[4] + r[5]; //addr 4 5 5
                break;
            case 22:
                r[5] = r[5] + 1; //addi 5 1 5
                break;
            case 23:
                r[5] = 25; //seti 25 1 5
                break;
            case 24:
                r[1] = r[1] + 1; //addi 1 1 1
                break;
            case 25:
                r[5] = 17; //seti 17 0 5
                break;
            case 26:
                r[2] = r[1]; //setr 1 7 2
                break;
            case 27:
                r[5] = 7; //seti 7 2 5
                break;
            case 28:
//                print_before ();
                r[1] = r[3] == r[0] ? 1 : 0; //eqrr 3 0 1
//                print_after ();
                endpoints.insert (std::make_pair (r[3], count));
//                std::cout << r[3] << " " << count << std::endl;
//                finished = true;
                break;
            case 29:
                r[5] = r[5] + r[1]; //addr 1 5 5
                break;
            case 30:
                r[5] = 5; //seti 5 5 5
                break;
            default:
                finished = true;
        }

        // if (count % 1000000 == 0)
        // {
            
        //    std::string ignore;
        //    std::getline (std::cin, ignore);

        // }

        if (count > 6000000000ull)
        {
            finished = true;
        }
    
        r[ip]++;
        ++count;
    }

    
//    std::cout << "Register 0 = " << r[0] << std::endl;

    for (auto& p : endpoints)
    {
        std::cout << p.second << " " << p.first << std::endl;
    }

    return 0;
}
