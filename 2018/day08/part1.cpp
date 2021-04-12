#include <iostream>
#include <fstream>
#include <vector>

struct Node
{
    std::size_t name = 0;
    std::vector<Node> children;
    std::vector<std::size_t> metadata;
};


std::size_t global_name = 1;

Node make_node (std::istream& in)
{
    std::size_t subnode_count = 0;
    std::size_t metadata_count = 0;

    in >> subnode_count >> metadata_count;

    Node node;

    node.name = global_name++;
    
    while (subnode_count > 0)
    {
        node.children.push_back (make_node (in));
        subnode_count--;
    }

    while (metadata_count > 0)
    {
        std::size_t metadata = 0;
        in >> metadata;

        node.metadata.push_back (metadata);
        metadata_count--;
    }

    return node;
}

std::ostream& print_node (std::ostream& out, const Node& node, std::size_t indent_count = 0)
{
    std::string indent (indent_count, ' ');
    out << indent <<  "Node metadata: ";
    for (auto& m : node.metadata)
    {
        out << m << ' ';
    }
    out << std::endl;
    out << indent << "Subnode count=" << node.children.size () << std::endl;
    for (auto& c : node.children)
    {
        print_node (out, c, indent_count + 3);
    }
    return out;
}

template <typename Func>
void visit (const Node& node, Func func)
{
    func(node);
    for (auto& c : node.children)
    {
        visit (c, func);
    }
}

std::size_t calc_value (const Node& node)
{
    std::size_t result = 0;

    if (node.children.empty ())
    {
        for (auto& m : node.metadata)
        {
            result += m;
        }
        std::cout << "metadata sum = " << result << std::endl;
    }
    else
    {
        for (auto& m : node.metadata)
        {
            auto index = m - 1;
            std::cout << "node " << node.name << " metadata index = " << index << " num children = " << node.children.size () << std::endl;
            try
            {
                result += calc_value(node.children.at (index));
            }
            catch (std::out_of_range)
            {
            }
        }
    }
    std::cout << "Returning: " << result << std::endl;
    return result;
}

int main (int argc, char* argv[])
{
    std::ifstream input { argv[1] };

    auto node = make_node (input);

    //print_node (std::cout, node);

    // std::size_t sum = 0;
    // visit (node,
    //        [&](const Node& n)
    //        {
    //            for (auto& m : n.metadata)
    //            {
    //                sum += m;
    //            }
    //        });

    // std::cout << "Sum = " << sum << std::endl;

    auto value = calc_value(node);
    std::cout << "Root Value = " << value << std::endl;

    return 0;
}
