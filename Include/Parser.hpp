#ifndef PARSER_HPP
#define PARSER_HPP
#include "Grammar.hpp"
#include "LR0Item.hpp"
#include <iomanip>
#include <stack>

class Parser
{
private:
    map<pair<int, string>, string> slr_table;
    string input;
    stack<string> parseStack;
    vector<string> tokenizeInput();

public:
    Parser();
    Parser(map<pair<int, string>, string> slr_table);
    map<pair<int, string>, string> getSLRTable();
    void getInput();
    bool parse();
};

Parser::Parser() {}

Parser::Parser(map<pair<int, string>, string> slr_table)
{
    this->slr_table = slr_table;
}

map<pair<int, string>, string> Parser::getSLRTable()
{
    return this->slr_table;
}

void Parser::getInput()
{
    cout << endl << "Enter the input string: ";
    getline(cin, this->input);
    this->input += " $";
}

vector<string> Parser::tokenizeInput()
{
    vector<string> tokens;
    string token = "";
    for (int i = 0; i < this->input.length(); i++)
    {
        if (this->input[i] == ' ')
        {
            tokens.push_back(token);
            token = "";
        }
        else
        {
            token += this->input[i];
        }
    }
    tokens.push_back(token);
    return tokens;
}

bool Parser::parse()
{
    parseStack.push("0");

    vector<string> tokens = tokenizeInput();

    cout << endl
         << "+"
         << "--------------------"
         << "-------------------"
         << "+"
         << "--------------------"
         << "-------------------"
         << "+"
         << "--------------------"
         << "-------------------"
         << "+" << endl;

    cout
        << setw(20) << "|"
        << setw(20) << "Stack"
        << setw(20) << "|"
        << setw(20) << "Action"
        << setw(20) << "|"
        << setw(20) << "Input Buffer"
        << setw(20) << "|" << endl;

    cout << "+"
         << "--------------------"
         << "-------------------"
         << "+"
         << "--------------------"
         << "-------------------"
         << "+"
         << "--------------------"
         << "-------------------"
         << "+" << endl;

    for (int i = 0; i < tokens.size(); i++)
    {
        string token = tokens[i];
        string top = parseStack.top();
        string action = slr_table[make_pair(stoi(top), token)];

        stack<string> temp = parseStack;
        vector<string> stack;
        string stackOutput = "";
        while (!temp.empty())
        {
            stack.push_back(temp.top());
            temp.pop();
        }
        for (int i = stack.size() - 1; i >= 0; i--)
        {
            stackOutput += stack[i] + " ";
        }

        string inputBuffer = "";
        for (int j = i; j < tokens.size(); j++)
        {
            inputBuffer += tokens[j] + " ";
        }

        cout << setw(20) << "|"
             << setw(20) << stackOutput
             << setw(20) << "|"
             << setw(20) << action
             << setw(20) << "|"
             << setw(20) << inputBuffer
             << setw(20) << "|" << endl;

        if (action == "") {
            cout << "+"
                 << "--------------------"
                 << "-------------------"
                 << "+"
                 << "--------------------"
                 << "-------------------"
                 << "+"
                 << "--------------------"
                 << "-------------------"
                 << "+" << endl;
            return false;
        }
        else if (action[0] == 'S')
        {
            parseStack.push(token);
            parseStack.push(action.substr(2, action.length() - 1));
        }
        else if (action[0] == 'R')
        {
            string rule = action.substr(2, action.length() - 1);
            string left = rule.substr(0, rule.find("->"));
            string right = rule.substr(rule.find("->") + 2, rule.length() - 1);
            for (int i = 0; i < right.length(); i++)
            {
                parseStack.pop();
                parseStack.pop();
            }
            string stackTopWhenPushing = parseStack.top();
            parseStack.push(left);
            parseStack.push(slr_table[make_pair(stoi(stackTopWhenPushing), left)]);
            i--;
        }
        else if (action == "A")
        {
            cout << "+"
                 << "--------------------"
                 << "-------------------"
                 << "+"
                 << "--------------------"
                 << "-------------------"
                 << "+"
                 << "--------------------"
                 << "-------------------"
                 << "+" << endl;
            return true;
        }
    }

    return false;
}

#endif