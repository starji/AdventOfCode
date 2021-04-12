#include <iostream>
#include <fstream>
#include <array>
#include <limits>
#include <vector>
#include <algorithm>
#include <tuple>

/*
 *  01234567890
 * 0
 * 1
 * 2
 * 3
 * 4
 * 5
 * 6
 * 7
 * 8
 * 9
 * 0
 */

using MapArray = std::array<std::array<char, 150>, 150>;

struct Cart
{
    Cart (int row, int col, int vel_row, int vel_col)
        : next (Turn::Left),
          row (row), col(col), vel_row(vel_row), vel_col(vel_col)
    {}

    enum class Turn { Left, Straight, Right };

    bool operator<(const Cart& other)
    {
        if (row < other.row)
            return true;
        else if (row == other.row)
            return col < other.col;
        return false;
    }

    bool operator==(const Cart& other)
    {
        return row == other.row && col == other.col;
    }

    bool alive = true;
    Turn next = Turn::Left;
    int row = 0, col = 0;
    int vel_row = 0, vel_col = 0;
};

char next (std::array<std::array<char, 150>, 150>& map, const Cart& cart)
{
    return map[cart.row + cart.vel_row][cart.col + cart.vel_col];
}

std::pair<int, int> turn_left (int vel_row, int vel_col)
{
    // -1, 0 -> 0, -1
    // 1, 0  -> 0, 1
    // 0, 1  -> -1, 0
    // 0, -1 -> 1, 0

    return std::make_pair (vel_col * -1, vel_row);
}

std::pair<int, int> turn_right (int vel_row, int vel_col)
{
    // -1, 0 -> 0, 1
    // 1, 0  -> 0, -1
    // 0, 1  -> 1, 0
    // 0, -1 -> -1, 0

    return std::make_pair (vel_col, vel_row * -1);
}

std::pair<int, int> turn_straight (int vel_row, int vel_col)
{
    return std::make_pair (vel_row, vel_col);
}

char cart_char (const Cart& cart)
{
    if (cart.vel_row > 0)
    {
        return 'v';
    }
    else if (cart.vel_row < 0)
    {
        return '^';
    }
    else if (cart.vel_col > 0)
    {
        return '>';
    }
    else if (cart.vel_col < 0)
    {
        return '<';
    }
    else
    {
        return 'X';
    }
}

char cart_debug (const Cart& cart)
{
    switch (cart.next)
    {
        case Cart::Turn::Left:
            return 'L';
        case Cart::Turn::Straight:
            return 'S';
        case Cart::Turn::Right:
            return 'R';
        default:
            return 'U';
    }
}

void print_overlay (const MapArray& map, const std::vector<Cart>& carts)
{
    auto iter = carts.begin ();

    for (auto i = 0; i < map.size (); ++i)
    {
        for (auto j = 0; j < map[i].size (); ++j)
        {
            if (iter != carts.end () && iter->row == i && iter->col == j)
            {
                std::cout << cart_debug (*iter);
                ++iter;
            }
            else
            {
                std::cout << map[i][j];
            }
        }
        std::cout << std::endl;
    }
}

int main (int argc, char* argv[])
{
    std::ifstream input (argv[1]);

    MapArray map;

    std::vector<Cart> carts;

    // Read the map
    for (auto& row : map)
    {
        input.getline (row.data (), 151, '\n');
    }

    // Find the carts
    for (auto i = 0; i < map.size (); ++i)
    {
        auto& row = map[i];
        auto cart_lambda = [](char c) { return c == '<' || c == '>' || c == '^' || c == 'v'; };
        auto iter = std::find_if (row.begin (), row.end (), cart_lambda);
        while (iter != row.end ())
        {
            switch (*iter)
            {
                case '<':
                    carts.push_back (Cart { i, static_cast<int>(std::distance (row.begin (), iter)), 0, -1 });
                    *iter = '-';
                    break;
                case '>':
                    carts.push_back (Cart { i, static_cast<int>(std::distance (row.begin (), iter)), 0, 1 });
                    *iter = '-';
                    break;
                case '^':
                    carts.push_back (Cart { i, static_cast<int>(std::distance (row.begin (), iter)), -1, 0 });
                    *iter = '|';
                    break;
                case 'v':
                    carts.push_back (Cart { i, static_cast<int>(std::distance (row.begin (), iter)), 1, 0 });
                    *iter = '|';
                    break;
            }
            ++iter;
        }
    }

    print_overlay (map, carts);

    std::size_t step = 0;
    bool broken = false;

    std::sort (carts.begin (), carts.end ());
    while (carts.size () > 1 && !broken)
    {
        for (auto& cart : carts)
        {
            if (cart.alive)
            {
                char next_char = map[cart.row + cart.vel_row][cart.col + cart.vel_col];
                
// Update cart position
//            std::cout << step << " pos=" << cart.row << ',' << cart.col << " vel=" << cart.vel_row << ',' << cart.vel_col << std::endl;
                cart.row += cart.vel_row;
                cart.col += cart.vel_col;

                for (auto& check_cart : carts)
                {
                    if (&cart != &check_cart &&
                        check_cart.alive &&
                        cart.row == check_cart.row &&
                        cart.col == check_cart.col)
                    {
                        cart.alive = false;
                        check_cart.alive = false;
                    }
                }
                
                // Turn based on the character we moved to...            
                switch (next_char)
                {
                    case '\\':
                        if (cart.vel_col > 0 || cart.vel_col < 0)
                        {
                            std::tie(cart.vel_row, cart.vel_col) = turn_right(cart.vel_row, cart.vel_col);
                        }
                        else if (cart.vel_row > 0 || cart.vel_row < 0)
                        {
                            std::tie(cart.vel_row, cart.vel_col) = turn_left(cart.vel_row, cart.vel_col);
                        }
                        break;
                    case '/':
                        if (cart.vel_col > 0 || cart.vel_col < 0)
                        {
                            std::tie(cart.vel_row, cart.vel_col) = turn_left(cart.vel_row, cart.vel_col);
                        }
                        else if (cart.vel_row > 0 || cart.vel_row < 0)
                        {
                            std::tie(cart.vel_row, cart.vel_col) = turn_right(cart.vel_row, cart.vel_col);
                        }
                        break;
                    case '+':
                        switch (cart.next)
                        {
                            case Cart::Turn::Left:
                                std::tie(cart.vel_row, cart.vel_col) = turn_left (cart.vel_row, cart.vel_col);
                                cart.next = Cart::Turn::Straight;
                                break;
                            case Cart::Turn::Straight:
                                std::tie(cart.vel_row, cart.vel_col) = turn_straight (cart.vel_row, cart.vel_col);
                                cart.next = Cart::Turn::Right;
                                break;
                            case Cart::Turn::Right:
                                std::tie(cart.vel_row, cart.vel_col) = turn_right (cart.vel_row, cart.vel_col);
                                cart.next = Cart::Turn::Left;
                                break;
                        }
                        
                        break;
                    case ' ':
                        std::cout << "Broken=" << " pos=" << cart.row << ',' << cart.col << " vel=" << cart.vel_row << ',' << cart.vel_col << std::endl;
                        broken = true;
                }
            }
        }

        std::sort (carts.begin (), carts.end ());

//        print_overlay (map, carts);

        auto check_alive = [](const Cart& cart)
                           {
                               return !cart.alive;
                           };

        std::vector<Cart>::iterator iter = carts.end ();
        while (((iter = std::find_if (carts.begin (), carts.end (), check_alive))) != carts.end ())
        {
            carts.erase (iter);
        }

        // auto iter = std::adjacent_find (carts.begin (), carts.end ());
        // while (iter != carts.end ())
        // {
        //     carts.erase (iter, iter + 2);
        //     iter = std::adjacent_find (carts.begin (), carts.end ());
        // }

        ++step;
    }

    for (auto& cart : carts)
    {
        std::cout << step << " pos=" << cart.row << ',' << cart.col << " vel=" << cart.vel_row << ',' << cart.vel_col << std::endl;
    }

        
    return 0;
}
