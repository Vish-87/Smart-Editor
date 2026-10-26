#pragma once
#include <string>
#include <sstream>
#include <cctype>
#include "TextEditor.h"
using namespace std;

inline string toUpper(string s) {
    for (char &c : s) {
        c = toupper(c);
    }
    return s;
}

inline string trim(string s) {
    size_t first = s.find_first_not_of(" \t\r\n");
    if (first == string::npos) {
        return "";
    }
    size_t last = s.find_last_not_of(" \t\r\n");
    return s.substr(first, last - first + 1);
}

inline string helpText() {
    return "Commands:\n"
           "  INSERT <text>              add text at the end\n"
           "  INSERT_AT <pos> <text>     add text at a position (0 = start)\n"
           "  DELETE <start> <length>    delete characters starting at 'start'\n"
           "  REPLACE <old> WITH <new>   replace the first occurrence\n"
           "  DISPLAY                    show the current text\n"
           "  UNDO                       undo the last operation\n"
           "  REDO                       redo the last undone operation\n"
           "  HISTORY                    show all editing operations\n"
           "  HELP                       show this list\n"
           "  EXIT                       quit";
}

// Takes one line the user typed, runs it on the editor, returns the reply text.
inline string handleCommand(TextEditor &editor, string line) {
    line = trim(line);
    if (line == "") {
        return "";
    }

    // Split into the first word (command) and everything after it
    string cmd, rest;
    size_t space = line.find(' ');
    if (space == string::npos) {
        cmd = line;
        rest = "";
    } else {
        cmd = line.substr(0, space);
        rest = trim(line.substr(space + 1));
    }
    cmd = toUpper(cmd);

    if (cmd == "INSERT") {
        if (rest == "") return "Usage: INSERT <text>";
        return editor.insert(rest);
    }

    if (cmd == "INSERT_AT") {
        istringstream in(rest);
        int pos;
        if (!(in >> pos)) return "Usage: INSERT_AT <position> <text>";
        string str;
        getline(in, str);
        str = trim(str);
        if (str == "") return "Usage: INSERT_AT <position> <text>";
        return editor.insertAt(pos, str);
    }

    if (cmd == "DELETE") {
        istringstream in(rest);
        int start, length;
        if (!(in >> start >> length)) return "Usage: DELETE <start> <length>";
        return editor.deleteText(start, length);
    }

    if (cmd == "REPLACE") {
        size_t w = toUpper(rest).find(" WITH ");
        if (w == string::npos) return "Usage: REPLACE <old> WITH <new>";
        string oldStr = trim(rest.substr(0, w));
        string newStr = trim(rest.substr(w + 6));
        if (oldStr == "" || newStr == "") return "Usage: REPLACE <old> WITH <new>";
        return editor.replace(oldStr, newStr);
    }

    if (cmd == "DISPLAY") return editor.display();
    if (cmd == "UNDO")    return editor.undo();
    if (cmd == "REDO")    return editor.redo();
    if (cmd == "HISTORY") return editor.showHistory();
    if (cmd == "HELP")    return helpText();

    return "Unknown command \"" + cmd + "\". Type HELP to see commands.";
}