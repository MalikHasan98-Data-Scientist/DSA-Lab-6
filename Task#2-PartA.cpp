// Task#2 Part A
#include <iostream>
#include <string>
using namespace std;

class Stack
{
public:
    char items[1000];
    int top;

    Stack()
    {
        top = -1;
    }
    void push(char c)
    {
        items[++top] = c;
    }
    char pop()
    {
        return items[top--];
    }
    bool isEmpty()
    {
        return top == -1;
    }
};
bool isValidBrackets(string s)
{
    Stack st;

    for (int i = 0; i < (int)s.length(); i++)
    {
        char c = s[i];

        if (c == '(' || c == '{' || c == '[')
        {
            st.push(c);
        }
        else if (c == ')' || c == '}' || c == ']')
        {
            if (st.isEmpty())
                return false;

            char opening = st.pop();

            if ((c == ')' && opening != '(') ||
                (c == '}' && opening != '{') ||
                (c == ']' && opening != '['))
            {
                return false;
            }
        }
    }
    return st.isEmpty();
}
int main()
{
    string s;

    cout << "Enter an expression: ";
    getline(cin, s);

    if (isValidBrackets(s))
        cout << "Brackets are balanced." << endl;
    else
        cout << "Brackets are not balanced." << endl;

    return 0;
}