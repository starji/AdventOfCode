#include <vector>

namespace op
{
    std::vector<std::size_t> addr (std::vector<std::size_t> registers, std::size_t index_a, std::size_t index_b, std::size_t output_index);
    std::vector<std::size_t> addi (std::vector<std::size_t> registers, std::size_t index_a, std::size_t value, std::size_t output_index);

    std::vector<std::size_t> mulr (std::vector<std::size_t> registers, std::size_t index_a, std::size_t index_b, std::size_t output_index);
    std::vector<std::size_t> muli (std::vector<std::size_t> registers, std::size_t index_a, std::size_t value, std::size_t output_index);

    std::vector<std::size_t> banr (std::vector<std::size_t> registers, std::size_t index_a, std::size_t index_b, std::size_t output_index);
    std::vector<std::size_t> bani (std::vector<std::size_t> registers, std::size_t index_a, std::size_t value, std::size_t output_index);

    std::vector<std::size_t> borr (std::vector<std::size_t> registers, std::size_t index_a, std::size_t index_b, std::size_t output_index);
    std::vector<std::size_t> bori (std::vector<std::size_t> registers, std::size_t index_a, std::size_t value, std::size_t output_index);

    std::vector<std::size_t> setr (std::vector<std::size_t> registers, std::size_t index_a, std::size_t ignore, std::size_t output_index);
    std::vector<std::size_t> seti (std::vector<std::size_t> registers, std::size_t index_a, std::size_t ignore, std::size_t output_index);

    std::vector<std::size_t> gtir (std::vector<std::size_t> registers, std::size_t value, std::size_t index_b, std::size_t output_index);
    std::vector<std::size_t> gtri (std::vector<std::size_t> registers, std::size_t index_a, std::size_t value, std::size_t output_index);
    std::vector<std::size_t> gtrr (std::vector<std::size_t> registers, std::size_t index_a, std::size_t index_b, std::size_t output_index);

    std::vector<std::size_t> eqir (std::vector<std::size_t> registers, std::size_t value, std::size_t index_b, std::size_t output_index);
    std::vector<std::size_t> eqri (std::vector<std::size_t> registers, std::size_t index_a, std::size_t value, std::size_t output_index);
    std::vector<std::size_t> eqrr (std::vector<std::size_t> registers, std::size_t index_a, std::size_t index_b, std::size_t output_index);
}
