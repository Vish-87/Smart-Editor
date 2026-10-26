#include <iostream>
#include "Commands.h"
using namespace std;

int main() {
    TextEditor editor;

    cout << "=== Smart Undo/Redo Text Editor ===" << endl;
    cout << "Type HELP for commands, EXIT to quit." << endl;

    string line;
    while (true) {
        cout << "> ";
        if (!getline(cin, line)) {
            break;   // input ended (e.g. Ctrl+D), so stop
        }
        if (toUpper(trim(line)) == "EXIT") {
            cout << "Goodbye!" << endl;
            break;
        }
        string reply = handleCommand(editor, line);
        if (reply != "") {
            cout << reply << endl;
        }
    }
    return 0;
}