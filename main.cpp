#include <iostream>
#include "TextEditor.h"
using namespace std;

int main() {
    TextEditor e;
    cout << e.insert("Data") << endl;
    cout << e.insert("Structures") << endl;
    cout << e.display() << endl;
    cout << e.undo() << endl;
    cout << e.display() << endl;
    cout << e.redo() << endl;
    cout << e.display() << endl;
    cout << e.replace("Data", "Algo") << endl;
    cout << e.display() << endl;
    cout << e.deleteText(0, 5) << endl;
    cout << e.display() << endl;
    cout << e.undo() << endl;
    cout << e.undo() << endl;
    cout << e.display() << endl;
    cout << e.showHistory() << endl;
    return 0;
}