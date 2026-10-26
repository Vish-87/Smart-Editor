#pragma once
#include <string>
#include "Operation.h"
using namespace std;

class DoublyLinkedList {
private:
    struct Node {
        Operation* op;
        Node* prev;
        Node* next;
        Node(Operation* o) {
            op = o;
            prev = nullptr;
            next = nullptr;
        }
    };

    Node* head;
    Node* current;

public:
    DoublyLinkedList() {
        head = nullptr;
        current = nullptr;
    }

    // FIX 3: uses a local pointer, so `current` is not overwritten
    ~DoublyLinkedList() {
        Node* temp = head;
        while (temp != nullptr) {
            Node* nextNode = temp->next;
            delete temp->op;
            delete temp;
            temp = nextNode;
        }
    }

    // FIX 1: two phases
    void add(Operation* op) {
        // Phase 1: delete the "future" (everything after current)
        Node* doomed;
        if (current == nullptr) {
            doomed = head;            // everything was undone, so delete the whole list
        } else {
            doomed = current->next;   // delete only what comes after current
        }
        while (doomed != nullptr) {
            Node* nextNode = doomed->next;   // save BEFORE deleting
            delete doomed->op;
            delete doomed;
            doomed = nextNode;
        }

        // Phase 2: attach the new node
        Node* newNode = new Node(op);
        if (current == nullptr) {
            head = newNode;               // new first node
        } else {
            current->next = newNode;
            newNode->prev = current;
        }
        current = newNode;
    }

    void undo() {
        if (current != nullptr) {
            current = current->prev;
        }
    }

    void redo() {
        if (current == nullptr) {
            current = head;
        } else if (current->next != nullptr) {
            current = current->next;
        }
    }

    // FIX 2: now shows numbers, the current marker, and (undone)
    string toString() {
        if (head == nullptr) {
            return "History is empty.";
        }

        string result = "";
        bool undone = (current == nullptr);   // if nothing is applied, everything is undone
        Node* temp = head;
        int i = 1;

        while (temp != nullptr) {
            result += to_string(i) + ". " + temp->op->describe();

            if (temp == current) {
                result += "  <-- current";
            } else if (undone) {
                result += " (undone)";
            }
            result += "\n";

            if (temp == current) {
                undone = true;   // everything after current is undone
            }
            temp = temp->next;
            i++;
        }
        return result;
    }
};