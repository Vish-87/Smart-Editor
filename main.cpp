#include <iostream>
#include <string>
#include "Stack.h"
#include "Operation.h"
using namespace std;

int main() {
     string text = "Hello, World!";
     Operation op("replace", 0,"Hello", "Hi");
     text = op.apply(text);
     cout << text << endl; // Output: "Hi, World!"

    Operation ins("INSERT", 4, "", "!!");
    text = ins.apply(text);
    cout << text << endl;      // expect?
    text = ins.reverse(text);
    cout << text << endl; 
    return 0;
}