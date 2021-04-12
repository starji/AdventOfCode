
using ShoppingList = Inventory;

ShoppingList makeList(ItemQuantity iq, const CraftingTree& tree)
{
    ShoppingList list;
    auto iter = std::find_if(tree.begin(), tree.end(),
                             [&](auto& p)
                             {
                                 return p.first.second == iq.second;
                             });
    if(iter != tree.end())
    {
        auto times = 1;
        if(iq.first > iter->first.first)
        {
            times += iq.first / iter->first.first;
        }

        for(auto& r : iter->second)
        {
            list[r.second] = r.first;
        }
    }
    else
    {
        list[iq.second] = iq.first;
    }
    return list;
}

std::vector<ItemQuantity> getBom(ItemQuantity iq, const CraftingTree& tree)
{
    std::vector<ItemQuantity> bom;
    auto iter = std::find_if(tree.begin(), tree.end(),
                             [&](auto& p)
                             {
                                 return p.first.second == iq.second;
                             });
    if(iter != tree.end())
    {
        auto times = 1;
        if(iq.first > iter->first.first)
        {
            times += iq.first / iter->first.first;
        }

        for(auto& r : iter->second)
        {
            bom.push_back({ times * r.first, r.second});_
        }
    }
    else
    {
        bom.push_back(iq);
    }
    return bom;
}

void process(ShoppingList& desired, ShoppingList& processed, Inventory& inventory, const CraftingTree& tree)
{
    for(auto& d : desired)
    {
        if(d.second > 0)
        {
            auto bom = getBom({d.second, d.first}, tree);
            for(auto& b : bom)
            {
                desired[b.second] = b.first;
            }
        }
    }
}
