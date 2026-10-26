#include <iostream>
#include "DoublyLinkedList.h"
using namespace std;

int main() {
    DoublyLinkedList h;
    h.add(new Operation("INSERT", 0, "", "Data"));
    h.add(new Operation("INSERT", 4, "", " Structures"));
    h.add(new Operation("REPLACE", 0, "Data", "Algo"));
    cout << h.toString() << endl;      // expect?

    h.undo();
    h.undo();
    cout << h.toString() << endl;      // expect?

    h.redo();
    cout << h.toString() << endl;      // expect?

    h.undo();
    h.add(new Operation("DELETE", 0, "Data", ""));
    cout << h.toString() << endl;      // expect?
    return 0;
}