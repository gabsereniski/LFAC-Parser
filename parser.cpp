#include <string.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <map>
#include <stack>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

#define REJECT 22

ifstream input;
ofstream output;
bool ended;
bool show_steps = 0;

bool valid(vector<string> v, string key) 
{ 
    return (find(v.begin(), v.end(), key) != v.end()); 
}

void clear_whitespaces(string &s)
{
    s.erase(remove_if(s.begin(), s.end(), ::isspace), s.end());
}

map<string, int> state_list = {
    { "0", 0 }, { "1", 1 }, { "2", 2 }, { "3", 3 },
    { "4", 4 }, { "5", 5 }, { "6", 6 }, { "7", 7 },
    { "8", 8 }, { "9", 9 }, {"10", 10}, {"11", 11},
    {"12", 12}, {"13", 13}, {"14", 14}, {"15", 15},
    {"16", 16}, {"17", 17}, {"18", 18}, {"19", 19},
    {"20", 20}, {"21", 21}, {"reject", 22}
};

vector<string> valid_tokens = {"num", "id", "+", "-", "*", "/", "=", "(", ")"};

int accept()
{
    cout << "ACCEPTED\n\n";
    ended = true;
    return 0;
}

string action_go_to(string state, string prod)
{
    if(state == "0")
    {
        if(prod == "E") return "2";
        if(prod == "F") return "3";
        if(prod == "S") return "4";
        if(prod == "T") return "5";
        if(prod == "V") return "6";
    }

    if(state == "1")
    {
        if(prod == "E") return "9";
        if(prod == "F") return "3";
        if(prod == "T") return "5";
        if(prod == "V") return "10";
    }

    if(state == "11") 
    {
        if(prod == "F") return "3";
        if(prod == "T") return "17";
        if(prod == "V") return "10";
    }

    if(state == "12")
    {
        if(prod == "F") return "3";
        if(prod == "T") return "18";
        if(prod == "V") return "10";
    }

    if(state == "13")
    {
        if(prod == "F") return "19";
        if(prod == "V") return "10";
    }

    if(state == "14")
    {
        if(prod == "F") return "20";
        if(prod == "V") return "10";
    }
    
    if(state == "15")
    {
        if(prod == "E") return "21";
        if(prod == "F") return "3";
        if(prod == "T") return "5";
        if(prod == "V") return "10";
    }

    if(show_steps) cout << "impossible combination of production and state\n";
    return "reject";
}

int go_to(stack<string> &s)
{
    string production = s.top();
    s.pop();
    string state = s.top();
    s.push(production);
    s.push(action_go_to(state, production));
    if(show_steps) cout << "go to " << s.top() << endl;
    return state_list[s.top()];
}

int reduce(stack<string> &s, int production)
{
    string top;
    if(show_steps) cout << "reduce by production ";
    switch(production)
    {
    case 1:
        if(show_steps) cout << "01 [S -> E]\n";
        s.pop(); //pop state
        s.pop(); //pop E
        s.push("S");
        break;
    
    case 2:
        if(show_steps) cout << "02 [S -> V=E]\n";
        do { top = s.top(), s.pop(); } while(top != "V");
        s.push("S");
        break;

    case 3:
        if(show_steps) cout << "03 [E -> E+T]\n";
        do { top = s.top(), s.pop(); } while(top != "E");
        s.push("E");
        break;
    
    case 4:
        if(show_steps) cout << "04 [E -> E-T]\n";
        do { top = s.top(), s.pop(); } while(top != "E");
        s.push("E");
        break;
    
    case 5:
        if(show_steps) cout << "05 [E -> T]\n";
        s.pop(); //pop state
        s.pop(); //pop T
        s.push("E");
        break;
    
    case 6:
        if(show_steps) cout << "06 [T -> T*F]\n";
        do { top = s.top(), s.pop(); } while(top != "T");
        s.push("T");
        break;
    
    case 7:
        if(show_steps) cout << "07 [T -> T/F]\n";
        do { top = s.top(), s.pop(); } while(top != "T");
        s.push("T");
        break;
    
    case 8:
        if(show_steps) cout << "08 [T -> F]\n";
        s.pop(); //pop state
        s.pop(); //pop F
        s.push("T");
        break;
    
    case 9:
        if(show_steps) cout << "09 [F -> (E)]\n";
        do { top = s.top(), s.pop(); } while(top != "(");
        s.push("F");
        break;
    
    case 10:
        if(show_steps) cout << "10 [F -> V]\n";
        s.pop(); //pop state
        s.pop(); //pop V
        s.push("F");
        break;
    
    case 11:
        if(show_steps) cout << "11 [F -> num]\n";
        s.pop(); //pop state
        s.pop(); //pop num
        s.push("F");
        break;
    
    case 12:
        if(show_steps) cout << "12 [V -> id]\n";
        s.pop(); //pop state
        s.pop(); //pop id
        s.push("V");
        break;
    }

    return go_to(s);
}

int shift(stack<string> &s, queue<string> &str, string state)
{
    s.push(str.front());
    s.push(state);
    str.pop();
    if(show_steps) cout << "shift to " << s.top() << endl;
    return state_list[s.top()];    
}

int q00(stack<string> &s, queue<string> &str)
{
    if(str.front() == "(")   return shift(s, str, "1");
    if(str.front() == "id")  return shift(s, str, "7");
    if(str.front() == "num") return shift(s, str, "8");
    return REJECT;
}

int q01(stack<string> &s, queue<string> &str)
{
    if(str.front() == "(")   return shift(s, str, "1");
    if(str.front() == "id")  return shift(s, str, "7");
    if(str.front() == "num") return shift(s, str, "8");
    return REJECT;
}

int q02(stack<string> &s, queue<string> &str)
{
    if(str.front() == "+") return shift(s, str, "11");
    if(str.front() == "-") return shift(s, str, "12");
    if(str.front() == "$") return reduce(s, 1);
    return REJECT;
}

int q03(stack<string> &s, queue<string> &str)
{
    if(str.front() == ")") return reduce(s, 8);
    if(str.front() == "+") return reduce(s, 8);
    if(str.front() == "-") return reduce(s, 8);
    if(str.front() == "*") return reduce(s, 8);
    if(str.front() == "/") return reduce(s, 8);
    if(str.front() == "$") return reduce(s, 8);
    return REJECT;    
}

int q04(stack<string> &s, queue<string> &str)
{
    if(str.front() == "$") return accept();
    return REJECT;    
}

int q05(stack<string> &s, queue<string> &str)
{
    if(str.front() == ")") return reduce(s, 5);
    if(str.front() == "+") return reduce(s, 5);
    if(str.front() == "-") return reduce(s, 5);
    if(str.front() == "*") return shift(s, str, "13");
    if(str.front() == "/") return shift(s, str, "14");
    if(str.front() == "$") return reduce(s, 5);
    return REJECT;    
}

int q06(stack<string> &s, queue<string> &str)
{
    if(str.front() == ")") return reduce(s, 10);
    if(str.front() == "+") return reduce(s, 10);
    if(str.front() == "-") return reduce(s, 10);
    if(str.front() == "*") return reduce(s, 10);
    if(str.front() == "/") return reduce(s, 10);
    if(str.front() == "=") return shift(s, str, "15");
    if(str.front() == "$") return reduce(s, 10);
    return REJECT;    
}

int q07(stack<string> &s, queue<string> &str)
{
    if(str.front() == ")") return reduce(s, 12);
    if(str.front() == "+") return reduce(s, 12);
    if(str.front() == "-") return reduce(s, 12);
    if(str.front() == "*") return reduce(s, 12);
    if(str.front() == "/") return reduce(s, 12);
    if(str.front() == "=") return reduce(s, 12);
    if(str.front() == "$") return reduce(s, 12);
    return REJECT;    
}

int q08(stack<string> &s, queue<string> &str)
{
    if(str.front() == ")") return reduce(s, 11);
    if(str.front() == "+") return reduce(s, 11);
    if(str.front() == "-") return reduce(s, 11);
    if(str.front() == "*") return reduce(s, 11);
    if(str.front() == "/") return reduce(s, 11);
    if(str.front() == "$") return reduce(s, 11);
    return REJECT;
}

int q09(stack<string> &s, queue<string> &str)
{
    if(str.front() == ")") return shift(s, str, "16");
    if(str.front() == "+") return shift(s, str, "11");
    if(str.front() == "-") return shift(s, str, "12");
    return REJECT;    
}

int q10(stack<string> &s, queue<string> &str)
{
    if(str.front() == ")") return reduce(s, 10);
    if(str.front() == "+") return reduce(s, 10);
    if(str.front() == "-") return reduce(s, 10);
    if(str.front() == "*") return reduce(s, 10);
    if(str.front() == "/") return reduce(s, 10);
    if(str.front() == "$") return reduce(s, 10);
    return REJECT;
}

int q11(stack<string> &s, queue<string> &str)
{
    if(str.front() == "(")   return shift(s, str, "1");
    if(str.front() == "id")  return shift(s, str, "7");
    if(str.front() == "num") return shift(s, str, "8");
    return REJECT;    
}

int q12(stack<string> &s, queue<string> &str)
{
    if(str.front() == "(")   return shift(s, str, "1");
    if(str.front() == "id")  return shift(s, str, "7");
    if(str.front() == "num") return shift(s, str, "8");
    return REJECT;    
}

int q13(stack<string> &s, queue<string> &str)
{
    if(str.front() == "(")   return shift(s, str, "1");
    if(str.front() == "id")  return shift(s, str, "7");
    if(str.front() == "num") return shift(s, str, "8");
    return REJECT;    
}

int q14(stack<string> &s, queue<string> &str)
{
    if(str.front() == "(")   return shift(s, str, "1");
    if(str.front() == "id")  return shift(s, str, "7");
    if(str.front() == "num") return shift(s, str, "8");
    return REJECT;    
}

int q15(stack<string> &s, queue<string> &str)
{
    if(str.front() == "(")   return shift(s, str, "1");
    if(str.front() == "id")  return shift(s, str, "7");
    if(str.front() == "num") return shift(s, str, "8");
    return REJECT;    
}

int q16(stack<string> &s, queue<string> &str)
{
    if(str.front() == ")") return reduce(s, 9);
    if(str.front() == "+") return reduce(s, 9);
    if(str.front() == "-") return reduce(s, 9);
    if(str.front() == "*") return reduce(s, 9);
    if(str.front() == "/") return reduce(s, 9);
    if(str.front() == "$") return reduce(s, 9);
    return REJECT;    
}

int q17(stack<string> &s, queue<string> &str)
{
    if(str.front() == ")") return reduce(s, 3);
    if(str.front() == "+") return reduce(s, 3);
    if(str.front() == "-") return reduce(s, 3);
    if(str.front() == "*") return shift(s, str, "13");
    if(str.front() == "/") return shift(s, str, "14");
    if(str.front() == "$") return reduce(s, 3);
    return REJECT;    
}

int q18(stack<string> &s, queue<string> &str)
{
    if(str.front() == ")") return reduce(s, 4);
    if(str.front() == "+") return reduce(s, 4);
    if(str.front() == "-") return reduce(s, 4);
    if(str.front() == "*") return shift(s, str, "13");
    if(str.front() == "/") return shift(s, str, "14");
    if(str.front() == "$") return reduce(s, 4);
    return REJECT;    
}

int q19(stack<string> &s, queue<string> &str)
{
    if(str.front() == ")") return reduce(s, 6);
    if(str.front() == "+") return reduce(s, 6);
    if(str.front() == "-") return reduce(s, 6);
    if(str.front() == "*") return reduce(s, 6);
    if(str.front() == "/") return reduce(s, 6);
    if(str.front() == "$") return reduce(s, 6);
    return REJECT;    
}

int q20(stack<string> &s, queue<string> &str)
{
    if(str.front() == ")") return reduce(s, 7);
    if(str.front() == "+") return reduce(s, 7);
    if(str.front() == "-") return reduce(s, 7);
    if(str.front() == "*") return reduce(s, 7);
    if(str.front() == "/") return reduce(s, 7);
    if(str.front() == "$") return reduce(s, 7);
    return REJECT;    
}

int q21(stack<string> &s, queue<string> &str)
{
    if(str.front() == "+") return shift(s, str, "11");
    if(str.front() == "-") return shift(s, str, "12");
    if(str.front() == "$") return reduce(s, 2);
    return REJECT;    
}

int reject(stack<string> &s, queue<string> &str)
{
    cout << "REJECTED\n\n";
    ended = true;
    return REJECT;
}

typedef int(*next_state)(stack<string> &s, queue<string> &str);

int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        cout << "Use: <input_file>\n";
        exit(0);
    }

    input.open(argv[1]);
   
    if(!input.good())
    {
        cout << "Error opening input file.\n";
        exit(0);
    }

    char q;
    cout << "display steps? (y/n) ";
    cin >> q;
    if(q == 'y' || q == 'Y') show_steps = 1;
    else show_steps = 0;
    cout << endl;
   
    stringstream strstream;
    strstream << input.rdbuf();
    string in_string = strstream.str();
    clear_whitespaces(in_string);
    
    string buff; 
    queue<string> str;
    
    string::iterator it = in_string.begin();
    while(it != in_string.end())
    {
        buff += *it++;
        if(valid(valid_tokens, buff))
        {
            str.push(buff);
            buff.clear();
        }
    }
    str.push("$");

    stack<string> s;
    s.push("$");
    s.push("0");

    next_state delta[] = {
        &q00, &q01, &q02, &q03, &q04, &q05, &q06, &q07, 
        &q08, &q09, &q10, &q11, &q12, &q13, &q14, &q15, 
        &q16, &q17, &q18, &q19, &q20, &q21, &reject
    };

    ended = false;
    int cur_state = state_list[s.top()];
    while(!ended)
    {
        cur_state = (*delta[cur_state])(s, str);
    }
}