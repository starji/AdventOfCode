#include <iostream>
#include <vector>
#include <fstream>

namespace advent
{
    class map
    {
    public:
        map(std::size_t rows, std::size_t columns)
            : rows(rows), columns(columns)
        {
            value.resize(rows);
            for (auto& row : value)
            {
                row.resize(columns);
            }
        }

        void add (std::size_t row, std::size_t col, std::size_t height, std::size_t width)
        {
            for (std::size_t i = 0; i < height; ++i)
            {
                for (std::size_t j = 0; j < width; ++j)
                {
                    value[row+i][col+j]++;
                }
            }
        }

        std::size_t countOverlap ()
        {
            std::size_t accum = 0;
            for (auto& row : value)
            {
                for (auto& col : row)
                {
                    if (col > 1)
                        ++accum;
                }
            }
            return accum;
        }

        const std::size_t rows, columns;
        std::vector<std::vector<std::size_t>> value;
    };


    struct ctype : public std::ctype<char>
    {
        ctype(std::size_t refs = 0)
            : std::ctype<char>(custom_table, false, refs)
        {
            std::copy(classic_table(), classic_table()+table_size, custom_table);
            custom_table['@'] |= space;
            custom_table['x'] |= space;
            custom_table[':'] |= space;
            custom_table['#'] |= space;
            custom_table[','] |= space;
        }
        
        mask custom_table[table_size];
    };
}


int main(int argc, char* argv[])
{
    advent::map cloth (1000, 1000);
    advent::ctype custom_ctype;

//    std::ifstream input (argv[1]);
    

    std::cin.imbue(std::locale (std::cin.getloc(), &custom_ctype));

    do
    {
        int id=0,row=0,col=0,height=0,width=0;
        std::cin >> id >> row >> col >> height >> width;

//        std::cout << '#' << id << " @ " << row << ',' << col << ": " << height << 'x' << width << std::endl;

        if (id != 0)
            cloth.add (row, col, height, width);
    } while(std::cin.good());


    std::cout << cloth.countOverlap () << std::endl;
}
