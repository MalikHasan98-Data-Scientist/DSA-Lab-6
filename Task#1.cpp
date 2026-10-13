#include <iostream>
#include <string>
#include <vector>
#include <cmath>
using namespace std;

// 1. Simple Stack Classes (No templates)
class StringStack {
private:
    string items[1000];
    int topIndex;
public:
    StringStack() { topIndex = -1; }
    bool isEmpty() { return topIndex == -1; }
    int size() { return topIndex + 1; }
    
    void push(string value) {
        if (topIndex == 999) { cout << "Error: Stack full!\n"; }
        else { items[++topIndex] = value; }
    }
    string pop() {
        if (isEmpty()) { cout << "Error: Stack empty!\n"; return ""; }
        return items[topIndex--];
    }
    string top() {
        if (isEmpty()) { cout << "Error: Stack empty!\n"; return ""; }
        return items[topIndex];
    }
};
class DoubleStack {
private:
    double items[1000];
    int topIndex;
public:
    DoubleStack() { topIndex = -1; }
    bool isEmpty() { return topIndex == -1; }
    int size() { return topIndex + 1; }
    
    void push(double value) {
        if (topIndex == 999) { cout << "Error: Stack full!\n"; }
        else { items[++topIndex] = value; }
    }
    double pop() {
        if (isEmpty()) { cout << "Error: Stack empty!\n"; return 0.0; }
        return items[topIndex--];
    }
};

// 2. Basic Helper Functions
bool isDigit(char c) {
    return c >= '0' && c <= '9';
}
bool isSpace(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}
bool isOperator(string token) {
    return token == "+" || token == "-" || token == "*" || token == "/" || token == "^";
}
int precedence(string op) {
    if (op == "^") return 3;
    if (op == "*" || op == "/") return 2;
    if (op == "+" || op == "-") return 1;
    return 0;
}
void addToken(string &result, string token) {
    if (!result.empty()) result += " ";
    result += token;
}
// Custom function to convert string to double
double stringToDouble(string s) {
    double result = 0.0;
    double decimalPart = 0.0;
    double decimalMultiplier = 1.0;
    bool isDecimal = false;
    bool isNegative = false;
    int start = 0;

    if (s[0] == '-') {
        isNegative = true;
        start = 1;
    }
    for (int i = start; i < (int)s.length(); i++) {
        if (s[i] == '.') {
            isDecimal = true;
        } else {
            int digit = s[i] - '0'; 
            if (!isDecimal) {
                result = result * 10 + digit;
            } else {
                decimalMultiplier *= 10;
                decimalPart = decimalPart * 10 + digit;
            }
        }
    }
    double finalValue = result + (decimalPart / decimalMultiplier);
    return isNegative ? -finalValue : finalValue;
}

// 3. Tokenizer
vector<string> tokenize(string expr) {
    vector<string> tokens;
    int i = 0;

    while (i < (int)expr.length()) {
        if (isSpace(expr[i])) {
            i++;
            continue;
        }
        bool negativeNumber = false;
        if (expr[i] == '-' && i + 1 < (int)expr.length() &&
            (isDigit(expr[i + 1]) || expr[i + 1] == '.')) {
            if (tokens.empty() || tokens.back() == "(" || isOperator(tokens.back())) {
                negativeNumber = true;
            }
        }
        if (isDigit(expr[i]) || expr[i] == '.' || negativeNumber) {
            int start = i;
            if (negativeNumber) i++;
            
            while (i < (int)expr.length() && (isDigit(expr[i]) || expr[i] == '.')) {
                i++;
            }
            tokens.push_back(expr.substr(start, i - start));
        }
        else if (expr[i] == '+' || expr[i] == '-' || expr[i] == '*' ||
                 expr[i] == '/' || expr[i] == '^' || expr[i] == '(' || expr[i] == ')') {
            tokens.push_back(string(1, expr[i]));
            i++;
        }else {
            cout << "Error: Invalid character '" << expr[i] << "' in expression!\n";
            return tokens; 
        }
    }
    return tokens;
}

// 4. Conversion Functions
string infix_to_postfix(string expr) {
    vector<string> tokens = tokenize(expr);
    StringStack operators;
    string result = "";

    for (string token : tokens) {
        if (token == "(") {
            operators.push(token);
        } 
        else if (token == ")") {
            while (!operators.isEmpty() && operators.top() != "(") {
                addToken(result, operators.pop());
            }
            if (operators.isEmpty()) {
                cout << "Error: Mismatched parentheses!\n";
                return "";
            }
            operators.pop(); 
        } 
        else if (isOperator(token)) {
            while (!operators.isEmpty() && operators.top() != "(" &&
                  (precedence(operators.top()) > precedence(token) ||
                  (precedence(operators.top()) == precedence(token) && token != "^"))) {
                addToken(result, operators.pop());
            }
            operators.push(token);
        } else {
            addToken(result, token);
        }
    }
    while (!operators.isEmpty()) {
        if (operators.top() == "(") {
            cout << "Error: Mismatched parentheses!\n";
            return "";
        }
        addToken(result, operators.pop());
    }
    return result;
}
string infix_to_prefix(string expr) {
    string postfix = infix_to_postfix(expr);
    if (postfix == "") return "";
    
    vector<string> tokens = tokenize(postfix);
    StringStack expressions;

    for (string token : tokens) {
        if (isOperator(token)) {
            if (expressions.size() < 2) {
                cout << "Error: Invalid expression!\n";
                return "";
            }
            string right = expressions.pop();
            string left = expressions.pop();
            expressions.push(token + " " + left + " " + right);
        } else {
            expressions.push(token);
        }
    }
    return expressions.pop();
}
string postfix_to_infix(string expr) {
    vector<string> tokens = tokenize(expr);
    StringStack expressions;

    for (string token : tokens) {
        if (isOperator(token)) {
            if (expressions.size() < 2) {
                cout << "Error: Invalid postfix expression!\n";
                return "";
            }
            string right = expressions.pop();
            string left = expressions.pop();
            expressions.push("(" + left + " " + token + " " + right + ")");
        } else {
            expressions.push(token);
        }
    }
    return expressions.pop();
}

// 5. Evaluation Functions
double calculate(double left, double right, string op) {
    if (op == "+") return left + right;
    if (op == "-") return left - right;
    if (op == "*") return left * right;
    if (op == "/") {
        if (right == 0) { 
            cout << "Error: Division by zero!\n"; 
            return 0.0; 
        }
        return left / right;
    }
    if (op == "^") return pow(left, right);

    cout << "Error: Unknown operator!\n";
    return 0.0;
}
double evaluate_postfix(string expr) {
    vector<string> tokens = tokenize(expr);
    DoubleStack values;
    for (string token : tokens) {
        if (isOperator(token)) {
            if (values.size() < 2) {
                cout << "Error: Invalid postfix expression!\n";
                return 0.0;
            }
            double right = values.pop();
            double left = values.pop();
            values.push(calculate(left, right, token));
        } else {
            values.push(stringToDouble(token));
        }
    }
    return values.pop();
}
double evaluate_prefix(string expr) {
    vector<string> tokens = tokenize(expr);
    DoubleStack values;

    for (int i = (int)tokens.size() - 1; i >= 0; i--) {
        string token = tokens[i];
        if (isOperator(token)) {
            if (values.size() < 2) {
                cout << "Error: Invalid prefix expression!\n";
                return 0.0;
            }
            double left = values.pop();
            double right = values.pop();
            values.push(calculate(left, right, token));
        } else {
            values.push(stringToDouble(token));
        }
    }
    return values.pop();
}
// 6. Main Function (UPDATED FOR USER INPUT)
int main() {
    string infix;
    // 1. Prompt the user
    cout << "Enter an infix expression (e.g., 12 + (3 * 4) - 5): ";
    // 2. Read the entire line, including spaces
    getline(cin, infix);
    // 3. Process and display the results
    string postfix = infix_to_postfix(infix);
    string prefix = infix_to_prefix(infix);

    cout << "\n--- Results ---" << endl;
    cout << "Infix Expression:   " << infix << endl;
    cout << "Postfix Expression: " << postfix << endl;
    cout << "Prefix Expression:  " << prefix << endl;
    if (postfix != "") {
        cout << "Postfix to Infix:   " << postfix_to_infix(postfix) << endl;
        cout << "Postfix Evaluation: " << evaluate_postfix(postfix) << endl;
        cout << "Prefix Evaluation:  " << evaluate_prefix(prefix) << endl;
    }
    return 0;
}