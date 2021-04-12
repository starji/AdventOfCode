#include <iostream>

namespace advent
{
    struct ctype : public std::ctype<char>
    {
        ctype(std::size_t refs = 0)
            : std::ctype<char>(custom_table, false, refs)
        {
            std::copy(classic_table(), classic_table()+table_size, custom_table);
            custom_table['@'] |= space;
            custom_table[':'] |= space;
            custom_table['#'] |= space;
            custom_table[','] |= space;
            custom_table['['] |= space;
            custom_table[']'] |= space;
//            custom_table['-'] |= space;
            custom_table['.'] |= space;
            custom_table['>'] |= space;
            custom_table['<'] |= space;
            custom_table['='] |= space;
            for (auto i = 'a' ; i <= 'z'; ++i)
            {
                custom_table[i] |= space;
            }
            for (auto i = 'A' ; i <= 'Z'; ++i)
            {
                custom_table[i] |= space;
            }
        }
        
        mask custom_table[table_size];
    };
}
