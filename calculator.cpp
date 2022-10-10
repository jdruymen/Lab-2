#include <cmath>
#include <iostream>
#include <string>

using namespace std;

//Computes the given "equation" with operators ^, *, /, +, -, returns the result as a string
string compute(string equation, int location) {
    //The operator we are going to use on the numbers (^, *, /, +, -)
    char op = equation.at(location);
    //The ending location of the second number
    int endOp = equation.find_first_not_of("1234567890.", location + 1);

    //The location of the operator before the first number we are operating on
    //This can be used to help ensure order of operations is followed, ie: 2+3*4, preOp would be the location of the +, one before the beginning of our first number
    int preOp = 0;

    //Finds the location of preOp by checking if the next operator is the one we are using, once this is true we have preOp
    while (equation.find_first_not_of("1234567890.", preOp + 1) != location) {
        preOp = equation.find_first_not_of("1234567890.", preOp + 1);
    }


    //Finds our first number we are operating on
    double num1;

    //If preOp is not 0, that means that we are operating on the first character after preOp
    if (preOp != 0)
        num1 = stod(equation.substr(preOp + 1, location - preOp));
    //Otherwise, we are operating on the first number of the equation (there is no operations before this number)
    else
        num1 = stod(equation.substr(0, location));
    
    double num2 = stod(equation.substr(location + 1, endOp - location - 1));


    string b;
    switch (op) {
    case '^':
        b = to_string(pow(num1, num2));
        break;
    case '*':
        b = to_string(num1 * num2);
        break;
    case '/':
        b = to_string(num1 / num2);
        break;
    case '+':
        b = to_string(num1 + num2);
        break;
    case '-':
        b = to_string(num1 - num2);
        break;
    default:
        "Error";
        break;
    }

    string a = "";
    if (preOp != 0)
        a = equation.substr(0, preOp + 1);

    string c = "";
    if (endOp != string::npos)
        c = equation.substr(endOp, equation.length());

    cout << "A: " << a << endl << "B: " << b << endl << "C: " << c << endl;

    return(a + b + c);
}

string solve(string equation) {
    //Deals with parenthesis and brackets
    int openP = equation.find_first_of("({");
    if (openP != string::npos) {
        int closeP;
        if (openP == equation.find_first_of('(')) {
            closeP = equation.find_first_of(')');
            while (equation.find_first_of('(', openP + 1) < closeP && equation.find_first_of('(', openP + 1) != string::npos)
                openP = equation.find_first_of('(', openP + 1);
        }
        else {
            closeP = equation.find_first_of('}');
            while (equation.find_first_of('{', openP + 1) < closeP && equation.find_first_of('{', openP + 1) != string::npos)
                openP = equation.find_first_of('{', openP + 1);
        }

        if (closeP == string::npos)
            return "Parenthesis/bracket error.";


        string a = equation.substr(0, openP);
        string b = solve(equation.substr(openP + 1, closeP - openP - 1));
        string c = equation.substr(closeP + 1, equation.length());

        return solve(a + b + c);
    }

    int trig = equation.find_first_of("sct");
    if (trig != string::npos) {
        string trigo = equation.substr(trig, 3);
        int endTrig = equation.find_first_not_of("1234567890.", trig + 3);
        double number;
        string b;

        if (endTrig != string::npos)
            number = stod(equation.substr(trig + 3, endTrig - trig - 3));
        else
            number = stod(equation.substr(trig + 3, equation.length()));

        if (trigo == "sin")
            b = to_string(sin(number));
        else if (trigo == "cos")
            b = to_string(cos(number));
        else if (trigo == "tan")
            b = to_string(tan(number));
        else if (trigo == "cot")
            b = to_string(cos(number) / sin(number));
        else
            return "Error";

        string a = equation.substr(0, trig);
        string c = "";
        if (endTrig != string::npos)
            c = equation.substr(endTrig, equation.length());

        return solve(a + b + c);
    }

    int operation = equation.find_first_of("^");
    while (operation != string::npos) {
        equation = compute(equation, operation);
        
        operation = equation.find_first_of('^');
    }
    
    operation = equation.find_first_of("*");
    while (operation != string::npos) {
        cout << equation << endl;
        equation = compute(equation, operation);

        operation = equation.find_first_of('*');
    }

    operation = equation.find_first_of("/");
    while (operation != string::npos) {
        equation = compute(equation, operation);

        operation = equation.find_first_of('/');
    }

    operation = equation.find_first_of("+");
    while (operation != string::npos) {
        equation = compute(equation, operation);

        operation = equation.find_first_of('+');
    }

    operation = equation.find_first_of("-");
    while (operation != string::npos) {
        equation = compute(equation, operation);

        operation = equation.find_first_of('-');
    }


    return equation;
}

int main() {
    string equation = "(2*3+4)/5";
    string solution = solve(equation);
    cout << solution << endl;
}