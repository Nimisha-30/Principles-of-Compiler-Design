#include "../include/Includes.hpp"
#include <iomanip>

// #define INPUT_FILE "input1.txt"
// #define INPUT_FILE "input2.txt"
#define INPUT_FILE "input3.txt"

void displaySLRTable(map<pair<int, string>, string>);

int main()
{
    Grammar G(INPUT_FILE);

    map<string, vector<string>> grammar = G.getAugmentedGrammarMap();

    cout << "Augmented Grammar" << endl;
    G.printAugmentedGrammar();
    cout << endl << endl;

    LR0Item item = LR0Item(G.getAugmentedStartSymbol(), grammar[G.getAugmentedStartSymbol()][0], 0);

    set<LR0Item> items;
    items.insert(item);

    set<LR0Item> closure_items = closure(items, G);

    map<pair<int, string>, string> slr_table;

    int state_count = 0;
    map<int, set<LR0Item>> states;
    states[0] = closure_items;

    int count = 0;

    while (count <= state_count)
    {
        set<LR0Item> s = states[count];

        for (const auto &item : s)
        {
            if (item.dot < item.right.size())
            {
                string next = item.right.substr(item.dot, 1);
                set<LR0Item> goto_items = goto_set(s, next, G);

                if (!gotoStateAlreadyExists(states, goto_items))
                {
                    states[++state_count] = goto_items;

                    if (next >= "A" && next <= "Z")
                        slr_table[make_pair(count, next)] = to_string(state_count);
                    else
                        slr_table[make_pair(count, next)] = "S:" + to_string(state_count);
                }
                else
                {
                    if (next >= "A" && next <= "Z")
                        slr_table[make_pair(count, next)] = to_string(getStateNumber(states, goto_items));
                    else
                        slr_table[make_pair(count, next)] = "S:" + to_string(getStateNumber(states, goto_items));
                }
            }
            else
            {
                if (item.left == G.getAugmentedStartSymbol())
                {
                    slr_table[make_pair(count, "$")] = "A";
                }
                else
                {
                    set<string> follow = G.getFollow()[item.left];
                    for (const auto &f : follow)
                    {
                        slr_table[make_pair(count, f)] = "R:" + item.left + "->" + item.right;
                    }
                }
            }
        }
        count++;
    }

    cout << "State count: " << state_count << endl << endl;

    displaySLRTable(slr_table);

    Parser p(slr_table);
    p.getInput();
    if(p.parse())
        cout << "Input accepted" << endl;
    else
        cout << "Input rejected" << endl;

    return 0;
}

void displaySLRTable(map<pair<int, string>, string> slr_table)
{
    cout << "SLR TABLE" << endl;
    set<int> row_values;
    set<string> col_values;

    for (auto const &key : slr_table)
    {
        row_values.insert(key.first.first);
        col_values.insert(key.first.second);
    }

    cout << left << setw(15) << " ";
    for (auto const &col : col_values)
    {
        cout << left << setw(15) << col;
    }
    cout << endl;
    cout << "---------------------------------------------------------------------------------------------------------------------------------------------" << endl;

    for (auto const &row : row_values)
    {
        cout << left << setw(15) << row;
        for (auto const &col : col_values)
        {
            auto it = slr_table.find({row, col});
            if (it != slr_table.end())
            {
                cout << left << setw(15) << it->second;
            }
            else
            {
                cout << left << setw(15) << " ";
            }
        }
        cout << endl;
    }
}