#pragma once
#include <string>
#include "Stack.h"
#include "Operation.h"
#include "DoublyLinkedList.h"
using namespace std;

class TextEditor {
private:
    string text;
    Stack<Operation*> undoStack;
    Stack<Operation*> redoStack;
    DoublyLinkedList history;

    // Every NEW edit goes through here
    void execute(Operation* op) {
        text = op->apply(text);
        undoStack.push(op);
        redoStack.clear();
        history.add(op);
    }

public:
    string insert(string str) {
        string toAdd = str;
        if (text.length() > 0) {
            toAdd = " " + str;
        }
        execute(new Operation("INSERT", text.length(), "", toAdd));
        return "Inserted \"" + str + "\".";
    }

    string insertAt(int pos, string str) {
        if (pos < 0 || pos > (int)text.length()) {
            return "Error: position must be between 0 and " + to_string(text.length()) + ".";
        }
        execute(new Operation("INSERT", pos, "", str));
        return "Inserted \"" + str + "\" at position " + to_string(pos) + ".";
    }

    string deleteText(int start, int length) {
        if (start < 0 || length <= 0 || start + length > (int)text.length()) {
            return "Error: invalid range. Text length is " + to_string(text.length()) + ".";
        }
        string removed = text.substr(start, length);
        execute(new Operation("DELETE", start, removed, ""));
        return "Deleted \"" + removed + "\".";
    }

    string replace(string oldStr, string newStr) {
        size_t idx = text.find(oldStr);
        if (idx == string::npos) {
            return "Error: \"" + oldStr + "\" not found in the text.";
        }
        execute(new Operation("REPLACE", (int)idx, oldStr, newStr));
        return "Replaced \"" + oldStr + "\" with \"" + newStr + "\".";
    }

    string undo() {
        Operation* op = undoStack.pop();
        if (op == nullptr) {
            return "Nothing to undo.";
        }
        text = op->reverse(text);
        redoStack.push(op);
        history.undo();
        return "Undid: " + op->describe();
    }

    string redo() {
        Operation* op = redoStack.pop();
        if (op == nullptr) {
            return "Nothing to redo.";
        }
        text = op->apply(text);
        undoStack.push(op);
        history.redo();
        return "Redid: " + op->describe();
    }

        string getText() {
        return text;
    }

    bool canUndo() {
        return !undoStack.isEmpty();
    }

    bool canRedo() {
        return !redoStack.isEmpty();
    }

    string display() {
        return "Current text: \"" + text + "\"";
    }

    string showHistory() {
        return history.toString();
    }
};