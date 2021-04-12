#include <iostream>
#include <array>

void print_registers (const std::array<std::size_t, 6>& registers)
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
        "addi 5 16 5",
        "seti 1 5 3 ",
        "seti 1 4 2 ",
        "mulr 3 2 4 ",
        "eqrr 4 1 4 ",
        "addr 4 5 5 ",
        "addi 5 1 5 ",
        "addr 3 0 0 ",
        "addi 2 1 2 ",
        "gtrr 2 1 4 ",
        "addr 5 4 5 ",
        "seti 2 2 5 ",
        "addi 3 1 3 ",
        "gtrr 3 1 4 ",
        "addr 4 5 5 ",
        "seti 1 0 5 ",
        "mulr 5 5 5 ",
        "addi 1 2 1 ",
        "mulr 1 1 1 ",
        "mulr 5 1 1 ",
        "muli 1 11 1",
        "addi 4 1 4 ",
        "mulr 4 5 4 ",
        "addi 4 9 4 ",
        "addr 1 4 1 ",
        "addr 5 0 5 ",
        "seti 0 5 5 ",
        "setr 5 6 4 ",
        "mulr 4 5 4 ",
        "addr 5 4 4 ",
        "mulr 5 4 4 ",
        "muli 4 14 4",
        "mulr 4 5 4 ",
        "addr 1 4 1 ",
        "seti 0 3 0 ",
        "seti 0 1 5 ",
    };
    std::array<std::size_t, 6> r { 1, 0, 0, 0, 0, 0 };

    std::uint64_t count = 0;
    bool finished = false;

    while (!finished)
    {

//        if (count % 1000000 == 0)
        {
            std::cout << "ip=" << r[ip] << ' ';
            print_registers (r);
            if (r[ip] < names.size ())
            {
                std::cout << ' ' << names[r[ip]] << ' ';
            }
        }

        switch (r[ip])
        {
            case 0:
                r[5] = r[5] + 16;            // addi 5 16 5
                break;
            case 1:
                r[3] = 1;                    // seti 1 5 3
                break;
            case 2:
                r[2] = 1;                    // seti 1 4 2
                break;
            case 3:
                r[4] = r[3] * r[2];          // mulr 3 2 4
                break;
            case 4:
                r[4] = r[4] == r[1] ? 1 : 0; // eqrr 4 1 4
                break;
            case 5:
                r[5] = r[4] + r[5];          // addr 4 5 5
                break;
            case 6:
                r[5] = r[5] + 1;             // addi 5 1 5
                break;
            case 7:
                r[0] = r[3] + r[0];          // addr 3 0 0
                break;
            case 8:
                r[2] = r[2] + 1;             // addi 2 1 2
                break;
            case 9:
                r[4] = r[2] > r[1] ? 1 : 0;  // gtrr 2 1 4
                break;
            case 10:
                r[5] = r[5] + r[4];          // addr 5 4 5
                break;
            case 11:
                r[5] = 2;                    // seti 2 2 5
                break;
            case 12:
                r[3] = r[3] + 1;             // addi 3 1 3
                break;
            case 13:
                r[4] = r[3] > r[1] ? 1 : 0;  // gtrr 3 1 4
                break;
            case 14:
                r[5] = r[4] + r[5];          // addr 4 5 5
                break;
            case 15:
                r[5] = 1;                    // seti 1 0 5
                break;
            case 16:
                r[5] = r[5] * r[5];          // mulr 5 5 5
                break;
            case 17:
                r[1] = r[1] + 2;             // addi 1 2 1
                break;
            case 18:
                r[1] = r[1] * r[1];          // mulr 1 1 1
                break;
            case 19:
                r[1] = r[5] * r[1];          // mulr 5 1 1
                break;
            case 20:
                r[1] = r[1] * 11;            // muli 1 11 1
                break;
            case 21:
                r[4] = r[4] + 1;             // addi 4 1 4
                break;
            case 22:
                r[4] = r[4] * r[5];          // mulr 4 5 4
                break;
            case 23:
                r[4] = r[4] + 9;             // addi 4 9 4
                break;
            case 24:
                r[1] = r[1] + r[4];          // addr 1 4 1
                break;
            case 25:
                r[5] = r[5] + r[0];          // addr 5 0 5
                break;
            case 26:
                r[5] = 0;                    // seti 0 5 5
                break;
            case 27:
                r[4] = r[5];                 // setr 5 6 4
                break;
            case 28:
                r[4] = r[4] * r[5];          // mulr 4 5 4
                break;
            case 29:
                r[4] = r[5] + r[4];          // addr 5 4 4
                break;
            case 30:
                r[4] = r[5] * r[4];          // mulr 5 4 4
                break;
            case 31:
                r[4] = r[4] * 14;            // muli 4 14 4
                break;
            case 32:
                r[4] = r[4] * r[5];          // mulr 4 5 4
                break;
            case 33:
                r[1] = r[1] + r[4];           // addr 1 4 1
                break;
            case 34:
                r[0] = 0;                    // seti 0 3 0
                break;
            case 35:
                r[5] = 0;                    // seti 0 1 5
                break;
            default:
                finished = true;
        }

//        if (count % 1000000 == 0)
        {
            print_registers (r);
            std::cout << std::endl;
            
           std::string ignore;
           std::getline (std::cin, ignore);

        }
        
        r[ip]++;
        ++count;
    }

    std::cout << "Register 0 = " << r[0] << std::endl;

    return 0;
}
