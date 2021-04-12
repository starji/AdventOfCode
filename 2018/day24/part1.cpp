#include <iostream>
#include <vector>
#include <algorithm>

enum class Type
{
    Slashing, Bludgeoning, Piercing, Cold, Fire, Radiation
};
    
struct Army
{
    Army (std::size_t unit_count, std::size_t hp_per_unit,
          std::initializer_list<Type> weaknesses,
          std::initializer_list<Type> immunities,
          std::size_t attack_damage, Type attack_type,
          std::size_t init)
        : unit_count(unit_count), hp_per_unit(hp_per_unit),
          weaknesses (weaknesses), immunities (immunities), 
          attack_damage (attack_damage), attack_type(attack_type),
          init(init)
    {}

    std::size_t effective_power () const { return unit_count * attack_damage; }

    bool operator<(const Army& other)
    {
        if (effective_power() < other.effective_power ())
        {
            return true;
        }
        else if (effective_power() == other.effective_power ())
        {
            return init < other.init;
        }
        else
        {
            return false;
        }
    }

    std::size_t unit_count;
    std::size_t hp_per_unit;

    std::size_t attack_damage;
    Type attack_type;

    std::vector<Type> weaknesses;
    std::vector<Type> immunities;

    std::size_t init;
};

std::ostream& operator<<(std::ostream& out, const Type& type)
{
    switch (type)
    {
        case Type::Slashing:
            out << "Slashing";
            break;
        case Type::Bludgeoning:
            out << "Bludgeoning";
            break;
        case Type::Piercing:
            out << "Piercing";
            break;
        case Type::Cold:
            out << "Cold";
            break;
        case Type::Fire:
            out << "Fire";
            break;
        case Type::Radiation:
            out << "Radiation";
            break;
    }
    return out;
}

std::ostream& operator<<(std::ostream& out, const Army& army)
{
    out << army.unit_count << ' '
        << army.hp_per_unit << ' '
        << army.attack_damage << ' '
        << army.attack_type << ' '
        << army.init << ' ';
    if (army.weaknesses.empty ())
    {
        out << "No Weaknesses ";
    }
    else
    {
        out << "Weaknesses: ";
        for (auto& w : army.weaknesses)
        {
            out << w << ' ';
        }
    }
    if (army.immunities.empty ())
    {
        out << "No Immunities ";
    }
    else
    {
        out << "Immunities: ";
        for (auto& w : army.immunities)
        {
            out << w << ' ';
        }
    }
    return out;
}

int main()
{
    std::vector<Army> immune {
        Army (479, 3393, { Type::Radiation }, {}, 66, Type::Cold, 8),
        Army (2202, 4950, { Type::Fire }, { Type::Slashing }, 18, Type::Cold, 2),
        Army (8132, 9680, { Type::Bludgeoning, Type::Fire }, { Type::Slashing }, 9, Type::Radiation, 7),
        Army (389, 13983, {}, { Type::Bludgeoning }, 256, Type::Cold, 13),
        Army (1827, 5107, {}, {}, 24, Type::Slashing, 18),
        Army (7019, 2261, {}, { Type::Radiation, Type::Slashing, Type::Cold }, 3, Type::Fire, 16 ),
        Army (4736, 8421, { Type::Cold }, {}, 17, Type::Slashing, 3),
        Army (491, 3518, { Type::Cold }, { Type::Fire, Type::Bludgeoning }, 65, Type::Radiation, 1),
        Army (2309, 7353, {}, { Type::Radiation }, 31, Type::Bludgeoning, 20),
        Army (411, 6375, { Type::Cold, Type::Fire }, { Type::Slashing }, 151, Type::Bludgeoning, 14)
    };

    std::vector<Army> infect {
        Army (148, 31914, { Type::Bludgeoning }, { Type::Radiation, Type::Cold, Type::Fire }, 416, Type::Cold, 4),
        Army (864, 38189, {}, {}, 72, Type::Slashing, 6),
        Army (2981, 7774, {}, { Type::Bludgeoning, Type::Cold }, 4, Type::Fire, 15),
        Army (5259, 22892, {}, {}, 8, Type::Fire, 5),
        Army (318, 16979, { Type::Fire }, {}, 106, Type::Bludgeoning, 9),
        Army (5017, 32175, { Type::Slashing }, { Type::Radiation }, 11, Type::Bludgeoning, 17),
        Army (4308, 14994, { Type::Slashing }, { Type::Fire, Type::Cold }, 5, Type::Fire, 10),
        Army (208, 14322, { Type::Radiation }, {}, 133, Type::Cold, 19),
        Army (3999, 48994, { Type::Cold, Type::Slashing }, {}, 20, Type::Cold, 11),
        Army (1922, 34406, { Type::Slashing }, {}, 35, Type::Slashing, 12),        
    };


    std::sort (immune.begin (), immune.end ());
    std::sort (infect.begin (), infect.end ());

    for (auto& army : immune)
    {
        std::cout << "Immune: " << army << std::endl;
    }
    for (auto& army : infect)
    {
        std::cout << "Infect: " << army << std::endl;
    }

    return 0;
}
