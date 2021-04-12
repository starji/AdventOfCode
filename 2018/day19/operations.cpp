#include <vector>
#include <stdexcept>

#include "operations.hpp"

namespace op
{
    void bounds_check (const std::vector<std::size_t>& registers, std::size_t index)
    {
        if (index >= registers.size ())
        {
            throw std::out_of_range ("Index out of range");
        }
    }

    std::vector<std::size_t> addr (std::vector<std::size_t> registers, std::size_t index_a, std::size_t index_b, std::size_t output_index)
    {
        bounds_check (registers, index_a);
        bounds_check (registers, index_b);
        bounds_check (registers, output_index);
        registers[output_index] = registers[index_a] + registers[index_b];
        return registers;
    }
    
    std::vector<std::size_t> addi (std::vector<std::size_t> registers, std::size_t index_a, std::size_t value, std::size_t output_index)
    {
        bounds_check (registers, index_a);
        bounds_check (registers, output_index);
        registers[output_index] = registers[index_a] + value;
        return registers;
    }

    std::vector<std::size_t> mulr (std::vector<std::size_t> registers, std::size_t index_a, std::size_t index_b, std::size_t output_index)
    {
        bounds_check (registers, index_a);
        bounds_check (registers, index_b);
        bounds_check (registers, output_index);
        registers[output_index] = registers[index_a] * registers[index_b];
        return registers;
    }
    
    std::vector<std::size_t> muli (std::vector<std::size_t> registers, std::size_t index_a, std::size_t value, std::size_t output_index)
    {
        bounds_check (registers, index_a);
        bounds_check (registers, output_index);
        registers[output_index] = registers[index_a] * value;
        return registers;
    }

    std::vector<std::size_t> banr (std::vector<std::size_t> registers, std::size_t index_a, std::size_t index_b, std::size_t output_index)
    {
        bounds_check (registers, index_a);
        bounds_check (registers, index_b);
        bounds_check (registers, output_index);
        registers[output_index] = registers[index_a] & registers[index_b];
        return registers;
    }
    std::vector<std::size_t> bani (std::vector<std::size_t> registers, std::size_t index_a, std::size_t value, std::size_t output_index)
    {
        bounds_check (registers, index_a);
        bounds_check (registers, output_index);
        registers[output_index] = registers[index_a] & value;
        return registers;
    }

    std::vector<std::size_t> borr (std::vector<std::size_t> registers, std::size_t index_a, std::size_t index_b, std::size_t output_index)
    {
        bounds_check (registers, index_a);
        bounds_check (registers, index_b);
        bounds_check (registers, output_index);
        registers[output_index] = registers[index_a] | registers[index_b];
        return registers;
    }
    
    std::vector<std::size_t> bori (std::vector<std::size_t> registers, std::size_t index_a, std::size_t value, std::size_t output_index)
    {
        bounds_check (registers, index_a);
        bounds_check (registers, output_index);
        registers[output_index] = registers[index_a] | value;
        return registers;
    }

    std::vector<std::size_t> setr (std::vector<std::size_t> registers, std::size_t index_a, std::size_t ignore, std::size_t output_index)
    {
        bounds_check (registers, index_a);
        bounds_check (registers, output_index);
        registers[output_index] = registers[index_a];
        return registers;
    }
    
    std::vector<std::size_t> seti (std::vector<std::size_t> registers, std::size_t value, std::size_t ignore, std::size_t output_index)
    {
        bounds_check (registers, output_index);
        registers[output_index] = value;
        return registers;
    }

    std::vector<std::size_t> gtir (std::vector<std::size_t> registers, std::size_t value, std::size_t index_b, std::size_t output_index)
    {
        bounds_check (registers, index_b);
        bounds_check (registers, output_index);
        registers[output_index] = (value > registers[index_b]) ? 1 : 0;
        return registers;
    }
    
    std::vector<std::size_t> gtri (std::vector<std::size_t> registers, std::size_t index_a, std::size_t value, std::size_t output_index)
    {
        bounds_check (registers, index_a);
        bounds_check (registers, output_index);
        registers[output_index] = (registers[index_a] > value) ? 1 : 0;
        return registers;
    }
    
    std::vector<std::size_t> gtrr (std::vector<std::size_t> registers, std::size_t index_a, std::size_t index_b, std::size_t output_index)
    {
        bounds_check (registers, index_a);
        bounds_check (registers, index_b);
        bounds_check (registers, output_index);
        registers[output_index] = (registers[index_a] > registers[index_b]) ? 1 : 0;
        return registers;
    }

    std::vector<std::size_t> eqir (std::vector<std::size_t> registers, std::size_t value, std::size_t index_b, std::size_t output_index)
    {
        bounds_check (registers, index_b);
        bounds_check (registers, output_index);
        registers[output_index] = (value == registers[index_b]) ? 1 : 0;
        return registers;
    }
    
    std::vector<std::size_t> eqri (std::vector<std::size_t> registers, std::size_t index_a, std::size_t value, std::size_t output_index)
    {
        bounds_check (registers, index_a);
        bounds_check (registers, output_index);
        registers[output_index] = (registers[index_a] == value) ? 1 : 0;
        return registers;
    }
    
    std::vector<std::size_t> eqrr (std::vector<std::size_t> registers, std::size_t index_a, std::size_t index_b, std::size_t output_index)
    {
        bounds_check (registers, index_a);
        bounds_check (registers, index_b);
        bounds_check (registers, output_index);
        registers[output_index] = (registers[index_a] == registers[index_b]) ? 1 : 0;
        return registers;
    }
}
