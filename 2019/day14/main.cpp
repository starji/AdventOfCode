#include <iostream>
#include <locale>
#include <map>
#include <iterator>
#include <cctype>
#include <algorithm>
#include <cstdlib>
#include <set>
#include <cmath>
#include <cstdint>
#include <limits>

#include "Advent.hpp"
#include "Intcode.hpp"

using ItemQuantity = std::pair<std::int64_t, std::string> ;
using CraftingTree = std::map<ItemQuantity, std::vector<ItemQuantity> >;

CraftingTree getExampleTree()
{
    CraftingTree tree;
    tree[{5,"NZVS"}] = { { 157, "ORE" } };
    tree[{6, "DCFZ"}] = { { 165, "ORE" } };
    tree[{1, "FUEL"}] = { { 44, "XJWVT"}, { 5, "KHKGT"}, { 1, "QDVJ"}, { 29, "NZVS"}, { 9, "GPVTF"}, { 48, "HKGWZ"} };
    tree[{9, "QDVJ"}] = { { 12, "HKGWZ"}, { 1, "GPVTF"}, { 8, "PSHF"} };
    tree[{7, "PSHF"}] = { { 179, "ORE"} };
    tree[{5, "HKGWZ"}] = { { 177, "ORE"} };
    tree[{2, "XJWVT"}] = { { 7, "DCFZ"}, { 7, "PSHF"} };
    tree[{2, "GPVTF"}] = { { 165, "ORE"} };
    tree[{8, "KHKGT"}] = { { 3, "DCFZ"}, { 7, "NZVS"}, { 5, "HKGWZ"}, { 10, "PSHF"} };
    return tree;
}

CraftingTree getTree ()
{
    CraftingTree tree;
    tree[{9, "PVTL"}] = { {2, "RWPCH"} };
    tree[{4, "BLPJK"}] = { {1, "FHFH" } };
    tree[{5, "VJNBT"}] = { {146, "ORE" } };
    tree[{5, "LKPQG"}] = { {8, "KDFNZ"},
                           {1, "ZJGH"},
                           {1,"GSCG"} };
    tree[{2, "BMJR"}] = { {11, "NWDZ"}, {2, "WBQR"}, {1, "VRQR"} };
    tree[{4, "KQDVM"}] = { {3, "GSCG" } };
    tree[{3, "BCNV"}] = { {5, "QVNKN"}, {6, "RPWKC"} };
    tree[{1, "JHXBM"}] = { {10, "QMBM"}, {4, "RBXB"}, {2, "VRQR"} };
    tree[{6, "MGCQ"}] = { {15, "RPWKC"} };
    tree[{4, "FHFH"}] = { {1, "QWKRZ"} };
    tree[{6, "MZKG"}] = { {10, "RWPCH"} };
    tree[{4, "VQDT"}] = { {11,"JFKGV"}, {5, "QVNKN"}, {1, "CTVK"} };
    tree[{5, "RPWKC"}] = { {1, "SXKT"} };
    tree[{2, "RBXB"}] = { {1, "VQDT"}, {25, "ZVMCB"} };
    tree[{3, "WBQR"}] = { {6, "LGLNV"}, {4, "XSNKB"} };
    tree[{2, "SXKT"}] = { {199, "ORE"} };
    tree[{3, "ZKTX"}] = { {1, "XSNKB"}, {6, "CWBNX"}, {1, "HPKB"}, {5, "PVTL"}, {1, "JNKH"}, {9, "SXKT"}, {3, "KQDVM"} };
    tree[{6, "WJDF"}] = { {7, "FDSX"} };
    tree[{4, "CWBNX"}] = { {7, "JLRM"} };
    tree[{5, "PQZXH"}] = { {167, "ORE"} };
    tree[{6, "DGFG"}] = { {13, "JHXBM"}, {2, "NWDZ"}, {4, "RFLX"}, {12, "VRQR"}, {10, "FJRFG"}, {14, "PVTL"}, {2, "JLRM"} };
    tree[{9, "ZJGH"}] = { {12, "HPKB"}, {3, "WHVXC"} };
    tree[{9, "FJRFG"}] = { {1, "JLRM"}, {2, "ZJGH"}, {2, "QVNKN"} };
    tree[{7, "KZFPJ"}] = { {129, "ORE"} };
    tree[{1, "RWPCH"}] = { {2, "QMBM"} };
    tree[{4, "JHDW"}] = { {7, "VJMWM"} };
    tree[{9, "BJVQM"}] = { {7, "PQZXH"}, {7, "SXKT"} };
    tree[{7, "FDSX"}] = { {1, "VJMWM"}, {4, "JHDW"}, {1, "MQXF"} };
    tree[{7, "WHVXC"}] = { {1, "RPWKC"} };
    tree[{1, "ZVMCB"}] = { {1, "ZJGH"} };
    tree[{3, "MPKR"}] = { {1, "RWPCH"} };
    tree[{8, "VJMWM"}] = { {187, "ORE"} };
    tree[{5, "GSCG"}] = { {15, "CTVK"} };
    tree[{2, "RFLX"}] = { {2, "XSNKB"}, {15, "ZVMCB"}, {3, "KDFNZ"} };
    tree[{8, "XLFZJ"}] = { {18, "QVNKN"} };
    tree[{8, "ZSCX"}] = { {4, "CWBNX"} };
    tree[{9, "NPRST"}] = { {2, "ZJGH"}, {1, "JLRM"}, {1, "MGCQ"} };
    tree[{2, "QWKRZ"}] = { {13, "BJVQM"}, {2, "BCNV"} };
    tree[{2, "VRQR"}] = { {2, "QWKRZ"}, {2, "BLPJK"}, {5, "XSNKB"} };
    tree[{9, "JLRM"}] = { {13, "HPKB"}, {3, "VQDT"} };
    tree[{6, "CTVK"}] = { {2, "SXKT"}, {1, "VJNBT"}, {5, "VLWQB"} };
    tree[{8, "DZFNW"}] = { {2, "MPKR"}, {2, "LMNCH"}, {24, "VRQR"} };
    tree[{1, "KDFNZ"}] = { {2, "VQDT"} };
    tree[{6, "QVNKN"}] = { {1, "CTVK"}, {6, "FDSX"} };
    tree[{4, "HPKB"}] = { {3, "CTVK"}, {1, "QVNKN"} };
    tree[{2, "QMBM"}] = { {3, "NPRST"}, {1, "KGSDJ"}, {1, "CTVK"} };
    tree[{5, "VLWQB"}] = { {4, "KZFPJ"}, {1, "PQZXH"} };
    tree[{7, "KGSDJ"}] = { {2, "VQDT"} };
    tree[{2, "JNKH"}] = { {3, "MPKR"} };
    tree[{5, "XQBS"}] = { {1, "KQDVM"} };
    tree[{6, "JQNF"}] = { {3, "ZKGMX"}, {1, "XQBS"}, {11, "MZKG"}, {11, "NPRST"}, {1, "DZFNW"}, {5, "VQDT"}, {2, "FHFH"} };
    tree[{1, "FUEL"}] = { {2, "FJRFG"}, {17, "BMJR"}, {3, "BJVQM"}, {55, "JQNF"}, {8, "DGFG"}, {13, "ZJGH"}, {29, "ZKTX"} };
    tree[{5, "MQXF"}] = { {27, "KZFPJ"}, {5, "VJNBT"} };
    tree[{4, "ZKGMX"}] = { {11, "FDSX"}, {1, "WHVXC"}, {1, "WJDF"} };
    tree[{4, "NWDZ"}] = { {1, "ZVMCB"} };
    tree[{6, "LGLNV"}] = { {1, "XLFZJ"} };
    tree[{8, "LMNCH"}] = { {13, "ZSCX"}, {4, "XLFZJ"} };
    tree[{8, "JFKGV"}] = { {1, "RPWKC"}, {1, "FDSX"}, {2, "BJVQM"} };
    tree[{4, "XSNKB"}] = { {1, "WJDF"}, {1, "LKPQG"} };
    return tree;
}

using Inventory = std::map<std::string, std::int64_t>;

bool build(ItemQuantity iq, Inventory& inv, const CraftingTree& tree)
{
    if(inv[iq.second] >= iq.first)
    {
        return true;
    }
    else
    {
        auto iter = std::find_if(tree.begin(), tree.end(),
                                 [&](auto& p)
                                 {
                                     return p.first.second == iq.second;
                                 });
        if(iter != tree.end())
        {
            auto quantity = iq.first - inv[iq.second];
            auto times = 1;
            if(quantity > iter->first.first)
            {
                times += (quantity / iter->first.first);
            }
            
            for(auto& r : iter->second)
            {
                if(build({times * r.first, r.second}, inv, tree))
                {
                    inv[r.second] -= times * r.first;
                }
                else
                {
                    return false;
                }
            }
            
            inv[iq.second] += times * iter->first.first;
        }
        else
        {
            return false;
        }
    }
    return true;
}

bool deconstruct(Inventory& inv, const CraftingTree& tree)
{
    bool result = false;
    for(auto& i : inv)
    {
        auto craft_iter = std::find_if(tree.begin(), tree.end(),
                                       [&](auto& p)
                                       {
                                           return p.first.second == i.first &&
                                               i.second >= p.first.first;
                                       });
        if(craft_iter != tree.end())
        {
            result = true;
            auto count = i.second / craft_iter->first.first;

            i.second -= count * craft_iter->first.first;

            for(auto& r : craft_iter->second)
            {
                inv[r.second] += count * r.first;
            }
        }
    }
    return result;
}

bool makeFuel(int to_make, Inventory& inv, const CraftingTree& tree, int& fuel_counter)
{
    while(deconstruct(inv, tree)) {}
    bool result = build({to_make, "FUEL"}, inv, tree);
    if(result)
    {
        inv["FUEL"] = 0;
        fuel_counter += to_make;
    }
    return result;
}

std::int64_t oreCount(std::int64_t fuel, const CraftingTree& tree)
{
    Inventory mats;
    mats["FUEL"] = fuel;
    while(deconstruct(mats, tree)) {};
    return mats["ORE"];
}

int main()
{
    auto tree = getTree();

    const auto INITIAL_ORE = 1000000000000ll;//std::numeric_limits<std::int64_t>::max();

    Inventory mats;
    // mats["ORE"] = INITIAL_ORE;
    
    // int fuel_counter = 0;
    // while(makeFuel(10000, mats, tree, fuel_counter)) {}
    // // try
    // // {
    // //     while(makeFuel(5000, mats, tree, fuel_counter))
    // //     {
    // //         if(fuel_counter % 1000 == 0)
    // //         {
    // //             std::cout << "Fuel=" << fuel_counter << std::endl;
    // //         }
    // //     }
    // // }
    // // catch(std::exception& e)
    // // {
    // //     std::cout << e.what () << std::endl;
    // //     while(deconstruct(mats, tree)) {}
    // // }

    // for(auto& p : mats)
    // {
    //     std::cout << p.first << " " << p.second << std::endl;
    // }


    // while(makeFuel(1, mats, tree, fuel_counter))
    // {
    //     if(fuel_counter % 100 == 0)
    //     {
    //         std::cout << "Fuel=" << fuel_counter << std::endl;
    //     }
    // }

    // for(auto& p : mats)
    // {
    //     std::cout << p.first << " " << p.second << std::endl;
    // }

//    std::int64_t fuel = 3279311;
    std::int64_t fuel = 1;

    std::cout << "Fuel=" << fuel << " Ore=" << oreCount(fuel, tree) << std::endl;
        
 


    return 0;
}
