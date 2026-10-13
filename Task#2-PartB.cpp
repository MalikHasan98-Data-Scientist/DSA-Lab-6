// Task #2 - Part B
#include <iostream>
#include <string>
using namespace std;

bool validate_markup(string s)
{
    string tags[100];
    int top = -1;
    bool valid = true;

    for (int i = 0; i < (int)s.length(); i++)
    {
        if (s[i] != '<')
            continue;

        int j = i + 1;

        while (j < (int)s.length() && s[j] != '>')
            j++;

        if (j == (int)s.length())
        {
            cout << "Error: Missing closing >" << endl;
            return false;
        }
        if (i + 1 < (int)s.length() &&
            (s[i + 1] == '!' || s[i + 1] == '?'))
        {
            i = j;
            continue;
        }
        bool closing = (s[i + 1] == '/');
        bool selfClosing = (!closing && s[j - 1] == '/');

        int k = i + 1;

        if (closing)
            k++;

        string name = "";

        // Read tag name without its attributes
        while (k < j && s[k] != ' ' &&
               s[k] != '/' && s[k] != '\t')
        {
            name += s[k];
            k++;
        }
        if (name == "")
        {
            cout << "Error: Empty tag found." << endl;
            valid = false;
        }
        else if (closing)
        {
            if (top == -1)
            {
                cout << "Error: Unexpected closing tag </"
                     << name << ">" << endl;
                valid = false;
            }
            else if (tags[top] != name)
            {
                cout << "Error: Expected </" << tags[top]
                     << "> but found </" << name << ">" << endl;
                valid = false;
            }
            else
            {
                top--;
            }
        }
        else if (!selfClosing)
        {
            if (top < 99)
                tags[++top] = name;
            else
            {
                cout << "Too many open tags." << endl;
                valid = false;
            }
        }
        i = j;
    }
    // Check for tags that were never closed
    while (top >= 0)
    {
        cout << "Error: Tag <" << tags[top]
             << "> was not closed." << endl;

        top--;
        valid = false;
    }
    return valid;
}
int main()
{
    string markup;

    cout << "Enter HTML/XML markup: ";
    getline(cin, markup);

    if (validate_markup(markup))
        cout << "Markup is valid." << endl;
    else
        cout << "Markup is invalid." << endl;

    return 0;
}