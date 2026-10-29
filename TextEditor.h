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

    bool isBreak(char c) {
        return c == ' ' || c == '\n';
    }

public:
    // ---------- INSERT ----------

    // Adds at the end. Puts a space before it, unless the text is empty,
    // already ends with a space/newline, or the new text starts with a newline.
    string insert(string str) {
        string toAdd = str;
        if (!text.empty() && !isBreak(text.back()) && str[0] != '\n') {
            toAdd = " " + str;
        }
        execute(new Operation("INSERT", text.length(), "", toAdd));
        return "Inserted \"" + str + "\".";
    }

    string newLine() {
        execute(new Operation("INSERT", text.length(), "", "\n"));
        return "Moved to a new line.";
    }

    string insertAt(int pos, string str) {
        if (pos < 0 || pos > (int)text.length()) {
            return "Error: position must be between 0 and " + to_string(text.length()) + ".";
        }
        execute(new Operation("INSERT", pos, "", str));
        return "Inserted \"" + str + "\" at position " + to_string(pos) + ".";
    }

    // INSERT_AFTER <word> <text>
    string insertAfter(string anchor, string str) {
        size_t idx = text.find(anchor);
        if (idx == string::npos) {
            return "Error: \"" + anchor + "\" not found in the text.";
        }
        int pos = (int)(idx + anchor.length());
        execute(new Operation("INSERT", pos, "", " " + str));
        return "Inserted \"" + str + "\" after \"" + anchor + "\".";
    }

    // INSERT_BEFORE <word> <text>
    string insertBefore(string anchor, string str) {
        size_t idx = text.find(anchor);
        if (idx == string::npos) {
            return "Error: \"" + anchor + "\" not found in the text.";
        }
        execute(new Operation("INSERT", (int)idx, "", str + " "));
        return "Inserted \"" + str + "\" before \"" + anchor + "\".";
    }

    // ---------- DELETE ----------

    // DELETE <text>: removes the first occurrence.
    // If it is a whole word, one neighbouring space is removed too.
    string deleteMatching(string target) {
        size_t idx = text.find(target);
        if (idx == string::npos) {
            return "Error: \"" + target + "\" not found in the text.";
        }
        size_t start = idx;
        size_t end = idx + target.length();

        bool startsWord = (idx == 0) || isBreak(text[idx - 1]);
        bool endsWord = (end == text.length()) || isBreak(text[end]);

        if (startsWord && endsWord) {
            if (start > 0 && text[start - 1] == ' ') {
                start--;
            } else if (end < text.length() && text[end] == ' ') {
                end++;
            }
        }

        string removed = text.substr(start, end - start);
        execute(new Operation("DELETE", (int)start, removed, ""));
        return "Deleted \"" + target + "\".";
    }

    // DELETE_RANGE <start> <length>  (the old number-based delete)
    string deleteRange(int start, int length) {
        if (start < 0 || length <= 0 || start + length > (int)text.length()) {
            return "Error: invalid range. Text length is " + to_string(text.length()) + ".";
        }
        string removed = text.substr(start, length);
        execute(new Operation("DELETE", start, removed, ""));
        return "Deleted \"" + removed + "\".";
    }

    // BACKSPACE [n]: removes the last n characters
    string backspace(int n) {
        if (text.empty()) {
            return "Error: the document is empty.";
        }
        if (n < 1) {
            return "Error: number of characters must be at least 1.";
        }
        if (n > (int)text.length()) {
            n = (int)text.length();
        }
        int start = (int)text.length() - n;
        string removed = text.substr(start, n);
        execute(new Operation("DELETE", start, removed, ""));
        return "Deleted the last " + to_string(n) + " character(s).";
    }

    // DELETE_WORD: removes the last word
    string deleteLastWord() {
        if (text.empty()) {
            return "Error: the document is empty.";
        }
        int end = (int)text.length();

        int i = end;                                   // skip trailing spaces/newlines
        while (i > 0 && isBreak(text[i - 1])) {
            i--;
        }
        int j = i;                                     // find where the word begins
        while (j > 0 && !isBreak(text[j - 1])) {
            j--;
        }
        int start = j;
        if (start > 0 && text[start - 1] == ' ') {     // also remove the space before it
            start--;
        }
        if (i == 0) {
            start = 0;                                 // text was only spaces/newlines
        }

        string removed = text.substr(start, end - start);
        execute(new Operation("DELETE", start, removed, ""));
        return "Deleted the last word.";
    }

    // CLEAR: removes everything (can still be undone)
    string clearAll() {
        if (text.empty()) {
            return "Error: the document is already empty.";
        }
        execute(new Operation("DELETE", 0, text, ""));
        return "Cleared the document.";
    }

    // ---------- REPLACE ----------

    string replace(string oldStr, string newStr) {
        size_t idx = text.find(oldStr);
        if (idx == string::npos) {
            return "Error: \"" + oldStr + "\" not found in the text.";
        }
        execute(new Operation("REPLACE", (int)idx, oldStr, newStr));
        return "Replaced \"" + oldStr + "\" with \"" + newStr + "\".";
    }

    // ---------- UNDO / REDO ----------

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

    // ---------- INFO ----------

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