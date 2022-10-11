#include <cmath>
#include <iostream>
#include <string>

using namespace std;

#define PI 3.14159265

//Removes excess trailing 0's generated from to_string(), taken from https://stackoverflow.com/questions/13686482/c11-stdto-stringdouble-no-trailing-zeros
string trim(string number) {
    number.erase(number.find_last_not_of('0') + 1, string::npos);
    number.erase(number.find_last_not_of('.') + 1, string::npos);

    return number;
}

//Computes the given "equation" with operators ^, *, /, +, -, returns the result as a string
string compute(string equation, int location) {
    //The operator we are going to use on the numbers (^, *, /, +, -)
    char op = equation.at(location);
    //The ending location of the second number
    int endOp = equation.find_first_not_of("1234567890.", location + 1);

    if (endOp == location + 1) {
        endOp = equation.find_first_not_of("1234567890.", endOp + 1);
    }

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

    //Second number will always begin one character after the operator until the next operator (or the rest of the equation if there is no other operator)
    double num2 = stod(equation.substr(location + 1, endOp - location - 1));
    
    cout << "Num1: " << num1 << endl << "Num 2: " << num2 << endl << "Op: " << op << endl;

    //Performs the operation and stores the result in string b
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

    //Saves the part (if any) of the equation before the operation
    string a = "";
    if (preOp != 0)
        a = equation.substr(0, preOp + 1);

    b = trim(b);

    //Saves the part (if any) of the equation after the operation
    string c = "";
    if (endOp != string::npos)
        c = equation.substr(endOp, equation.length());

    //Returns the new equation with part "b" computed
    return(a + b + c);
}

//Solves the given equation and returns the result as a string
string solve(string equation) {
    //Deals with parenthesis and brackets
    //Checks if there is any opening parenthesis/brackets
    int openP = equation.find_first_of("({");
    if (openP != string::npos) {
        //Determines if it was parenthesis or brackets
        int closeP;
        if (openP == equation.find_first_of('(')) {
            //Checks if the first closing parenthesis matches with the first opening parenthesis
            closeP = equation.find_first_of(')');
            while (equation.find_first_of('(', openP + 1) < closeP && equation.find_first_of('(', openP + 1) != string::npos)
                //If they do not match, moves the opening parenthesis marker to the next one it finds until they do match.
                openP = equation.find_first_of('(', openP + 1);
        }
        else {
            closeP = equation.find_first_of('}');
            while (equation.find_first_of('{', openP + 1) < closeP && equation.find_first_of('{', openP + 1) != string::npos)
                openP = equation.find_first_of('{', openP + 1);
        }

        //If there is no closing parenthesis that matches the opener, gives an error.
        if (closeP == string::npos)
            return "Parenthesis/bracket error. Not enough closing parenthesis/incorrect order.";

        //Saves everything before the selected opening parenthesis
        string a = equation.substr(0, openP);
        //Saves the result of everything inside of the parenthesis
        string b = solve(equation.substr(openP + 1, closeP - openP - 1));
        //Saves everything after the the selected closing parenthesis
        string c = equation.substr(closeP + 1, equation.length());

        //Solves the rest of the equation after this set of parenthesis has been dealt with
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
            b = to_string(sin(number * PI / 180.0));
        else if (trigo == "cos")
            b = to_string(cos(number * PI / 180.0));
        else if (trigo == "tan")
            b = to_string(tan(number * PI / 180.0));
        else if (trigo == "cot")
            b = to_string(cos(number * PI / 180.0) / sin(number * PI / 180.0));
        else
            return "Error in input: only sin, cos, tan, cot, ln, and log are accepted (case sensitive).";

        string a = equation.substr(0, trig);
        string c = "";
        if (endTrig != string::npos)
            c = equation.substr(endTrig, equation.length());

        b = trim(b);

        return solve(a + b + c);
    }

    int logLocation = equation.find_first_of("l");
    if (logLocation != string::npos) {
        bool natural;
        int length;
        if (equation.at(logLocation + 1) == 'n') {
            natural = true;
            length = 2;
        }
        else if (equation.at(logLocation + 1) == 'o' && equation.at(logLocation + 2) == 'g') {
            natural = false;
            length = 3;
        }
        else {
            return "Error in input: only sin, cos, tan, cot, ln, and log are accepted (case sensitive).";
        }

        double number;

        int endLog = equation.find_first_not_of("1234567890.", logLocation + length);
        
        if (endLog != string::npos)
            number = stod(equation.substr(logLocation + length, endLog - logLocation - length));
        else
            number = stod(equation.substr(logLocation + length, equation.length()));

        string b;
        if (natural)
            b = to_string(log(number));
        else
            b = to_string(log10(number));

        string a = equation.substr(0, logLocation);
        string c = "";
        if (endLog != string::npos)
            c = equation.substr(endLog, equation.length());
        
        b = trim(b);

        return solve(a + b + c);
    }

    int operation = equation.find_first_of("-");
    while (operation != string::npos) {
        if (operation != equation.length()) {
            if (equation.at(operation + 1) == '-') {
                equation.replace(operation, 2, "+");
                operation = equation.find_first_of("-");
            }

            else
                operation = equation.find_first_of("-", operation + 1);
        }
        else
            break;
    }

    operation = equation.find_first_of("^");
    while (operation != string::npos) {
        equation = compute(equation, operation);
        
        operation = equation.find_first_of('^');
    }
    
    operation = equation.find_first_of("*");
    while (operation != string::npos) {
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
        if (operation == 0)
            equation.erase(0);
        else
            equation = compute(equation, operation);

        operation = equation.find_first_of('+');
    }

    operation = equation.find_first_of("-");
    while (operation != string::npos) {
        if (operation == 0) {
            operation = equation.find_first_of('-', operation + 1);
            continue;
        }

        else
            equation = compute(equation, operation);

        operation = equation.find_first_of('-');
    }


    return equation;
}

int main() {
    string equation = "-5.78+-(4-2.23)+sin(0)*cos(1)/(1+tan(2*ln(-3+2*(1.23+99.111))))";
    string solution = solve(equation);
    cout << "Result: " << solution << endl;
}