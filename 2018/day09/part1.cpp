#include <iostream>
#include <vector>
#include <tuple>
#include <map>

struct Node
{
    std::uint64_t value;
    Node* next;
    Node* prev;
};

Node* insert (std::uint64_t value, Node* next)
{
    auto* new_node = new Node;
    new_node->value = value;
    new_node->next = next;
    new_node->prev = next->prev;

    next->prev->next = new_node;
    next->prev = new_node;

    return new_node;
}

Node* erase (Node* node)
{
    node->next->prev = node->prev;
    node->prev->next = node->next;

    auto next = node->next;

    // Here or elsewhere?
    delete node;

    return next;
}

Node* next (Node* node, std::uint64_t amount)
{
    while (amount--)
    {
        node = node->next;
    }
    return node;
}

Node* prev (Node* node, std::uint64_t amount)
{
    while (amount--)
    {
        node = node->prev;
    }
    return node;
}

// Step
Node* step (std::uint64_t value, Node* current)
{
    return insert (value, next(current, 2));
}

std::pair<Node*, std::uint64_t> scoring_step (Node* current)
{
    auto to_remove = prev(current, 7);
    auto value = to_remove->value;
    return std::make_pair (erase(to_remove), value);
}

void print (Node* head, Node* current, std::uint64_t player)
{
    std::cout << '[' << player << ']' << ' ';
    Node* iter = head;
    do
    {
        if (iter == current)
        {
            std::cout << '(' << iter->value << ')' << ' ';
        }
        else
        {
            std::cout << iter->value << ' ';
        }
        iter = iter->next;
    } while (iter != head);
    std::cout << std::endl;
}

std::map<std::uint64_t, std::uint64_t> play_until (std::uint64_t marble_point, std::uint64_t num_players)
{
    // Initialize
    Node* head = new Node;
    head->value = 0;
    head->next = head;
    head->prev = head;

    Node* current = head;
    std::uint64_t round = 1;
    std::uint64_t marble = 1;
    std::uint64_t player = 0;
    std::map<std::uint64_t, std::uint64_t> scores;
    bool game_over = false;
    while (marble <= marble_point)
    {
        if (marble % 23 == 0)
        {
            std::uint64_t score = 0;
            std::tie(current, score) = scoring_step (current);
            auto round_score = score + marble;
            scores[player] += round_score;
            marble++;
        }
        else
        {
            current = step (marble++, current);
        }

//        print (head, current, player);
        player = (player + 1) % num_players;
    }

    // Cleanup
    Node* iter = head;
    do
    {
        auto next = iter->next;
        delete iter;
        iter = next;
    } while (iter != head);

    return scores;
}
    
int main (int argc, char* argv[])
{
    auto scores = play_until (7130700, 458);

    std::uint64_t highest = 0;
    for (auto& s : scores)
    {
        highest = std::max (highest, s.second);
    }

    std::cout << "Highest score=" << highest;

    return 0;
}
