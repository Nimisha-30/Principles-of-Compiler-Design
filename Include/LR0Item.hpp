#ifndef LR0ITEM_HPP
#define LR0ITEM_HPP

#include "Grammar.hpp"
using namespace std;

class LR0Item
{
public:
    string left;
    string right;
    int dot;

    bool operator==(const LR0Item &other) const
    {
        return left == other.left && right == other.right && dot == other.dot;
    }

    bool operator<(const LR0Item &other) const
    {
        if (right != other.right)
            return right < other.right;
        else
            return dot < other.dot;
    }

    LR0Item() {}

    LR0Item(string left, string right, int dot)
    {
        this->left = left;
        this->right = right;
        this->dot = dot;
    }
};

void displayLR0Items(set<LR0Item> items)
{
    for (const auto &item : items)
    {
        cout << item.left << " -> " << item.right.substr(0, item.dot) << "." << item.right.substr(item.dot) << endl;
    }
}

set<LR0Item> closure(set<LR0Item> items, Grammar &G)
{
    map<string, vector<string>> grammar = G.getAugmentedGrammarMap();
    set<LR0Item> closure_items = items;

    while (true)
    {
        bool added = false;
        for (const auto &item : closure_items)
        {
            if (item.dot >= item.right.size())
            {
                continue;
            }
            string next = item.right.substr(item.dot, 1);
            for (const auto &production : grammar[next])
            {
                LR0Item new_item = LR0Item(next, production, 0);
                bool flag = true;
                for (const auto &item : closure_items)
                {
                    if (item == new_item)
                    {
                        flag = false;
                        break;
                    }
                }
                if (flag)
                {
                    added = true;
                    closure_items.insert(new_item);
                }
            }
        }
        if (!added)
            break;
    }
    return closure_items;
}

set<LR0Item> goto_set(set<LR0Item> items, string symbol, Grammar &G)
{
    set<LR0Item> goto_items;

    map<string, vector<string>> grammar = G.getAugmentedGrammarMap();

    for (const auto &item : items)
    {
        if (item.dot >= item.right.size())
        {
            continue;
        }

        string next = item.right.substr(item.dot, 1);

        if (next == symbol)
        {
            LR0Item new_item = LR0Item(item.left, item.right, item.dot + 1);
            goto_items.insert(new_item);
        }
    }
    set<LR0Item> closure_items = closure(goto_items, G);

    return closure_items;
}

bool gotoStateAlreadyExists(map<int, set<LR0Item>> states, set<LR0Item> state)
{
    bool gotoStateExists = false;
    for (int i = 0; i < states.size(); i++)
    {
        if (states[i] == state)
        {
            gotoStateExists = true;
            break;
        }
    }

    return gotoStateExists;
}

int getStateNumber(map<int, set<LR0Item>> states, set<LR0Item> state)
{
    int stateNumber = -1;
    for (int i = 0; i < states.size(); i++)
    {
        if (states[i] == state)
        {
            stateNumber = i;
            break;
        }
    }

    return stateNumber;
}

#endif