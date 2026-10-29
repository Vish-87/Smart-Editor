#pragma once
#include <string>
#include <sstream>
#include <cctype>
#include "TextEditor.h"
using namespace std;

inline string toUpper(string s) {
    for (char &c : s) {
        c = toupper((unsigned char)c);
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

// Turns the two typed characters \ and n into a real newline
inline string unescapeNewlines(string s) {
    string out = "";
    for (size_t i = 0; i < s.length(); i++) {
        if (s[i] == '\\' && i + 1 < s.length() && s[i + 1] == 'n') {
            out += '\n';
            i++;
        } else {
            out += s[i];
        }
    }
    return out;
}

// Splits "word rest of the line" into first = "word", rest = "rest of the line"
inline bool splitFirst(string s, string &first, string &rest) {
    s = trim(s);
    size_t space = s.find(' ');
    if (s == "" || space == string::npos) {
        return false;
    }
    first = s.substr(0, space);
    rest = trim(s.substr(space + 1));
    return rest != "";
}

inline string helpText() {
    return "Commands:\n"
           "  INSERT <text>                 add text at the end (use \\n for a new line)\n"
           "  NEWLINE                       go to the next line\n"
           "  INSERT_AFTER <word> <text>    add text after a word\n"
           "  INSERT_BEFORE <word> <text>   add text before a word\n"
           "  INSERT_AT <pos> <text>        add text at a character position\n"
           "  DELETE <text>                 delete the first match of that text\n"
           "  DELETE_WORD                   delete the last word\n"
           "  BACKSPACE [n]                 delete the last n characters (default 1)\n"
           "  DELETE_RANGE <start> <len>    delete by character position\n"
           "  CLEAR                         delete everything\n"
           "  REPLACE <old> WITH <new>      replace the first match\n"
           "  DISPLAY                       show the current text\n"
           "  UNDO / REDO                   undo or redo\n"
           "  HISTORY                       show all editing operations\n"
           "  HELP                          show this list\n"
           "  EXIT                          quit (terminal version)";
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
        return editor.insert(unescapeNewlines(rest));
    }

    if (cmd == "NEWLINE") {
        return editor.newLine();
    }

    if (cmd == "INSERT_AFTER" || cmd == "INSERT_BEFORE") {
        string anchor, str;
        if (!splitFirst(rest, anchor, str)) {
            return "Usage: " + cmd + " <word> <text>";
        }
        str = unescapeNewlines(str);
        if (cmd == "INSERT_AFTER") return editor.insertAfter(anchor, str);
        return editor.insertBefore(anchor, str);
    }

    if (cmd == "INSERT_AT") {
        istringstream in(rest);
        int pos;
        if (!(in >> pos)) return "Usage: INSERT_AT <position> <text>";
        string str;
        getline(in, str);
        str = trim(str);
        if (str == "") return "Usage: INSERT_AT <position> <text>";
        return editor.insertAt(pos, unescapeNewlines(str));
    }

    if (cmd == "DELETE") {
        if (rest == "") return "Usage: DELETE <text>";
        return editor.deleteMatching(rest);
    }

    if (cmd == "DELETE_WORD") {
        return editor.deleteLastWord();
    }

    if (cmd == "BACKSPACE") {
        int n = 1;
        if (rest != "") {
            istringstream in(rest);
            if (!(in >> n)) return "Usage: BACKSPACE [number]";
        }
        return editor.backspace(n);
    }

    if (cmd == "DELETE_RANGE") {
        istringstream in(rest);
        int start, length;
        if (!(in >> start >> length)) return "Usage: DELETE_RANGE <start> <length>";
        return editor.deleteRange(start, length);
    }

    if (cmd == "CLEAR") {
        return editor.clearAll();
    }

    if (cmd == "REPLACE") {
        size_t w = toUpper(rest).find(" WITH ");
        if (w == string::npos) return "Usage: REPLACE <old> WITH <new>";
        string oldStr = trim(rest.substr(0, w));
        string newStr = trim(rest.substr(w + 6));
        if (oldStr == "" || newStr == "") return "Usage: REPLACE <old> WITH <new>";
        return editor.replace(oldStr, unescapeNewlines(newStr));
    }

    if (cmd == "DISPLAY") return editor.display();
    if (cmd == "UNDO")    return editor.undo();
    if (cmd == "REDO")    return editor.redo();
    if (cmd == "HISTORY") return editor.showHistory();
    if (cmd == "HELP")    return helpText();

    return "Unknown command \"" + cmd + "\". Type HELP to see commands.";
}