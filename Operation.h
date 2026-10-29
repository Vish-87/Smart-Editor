#pragma once
#include <string>
using namespace std;

class Operation {
public:
    string type;       // "INSERT", "DELETE" or "REPLACE"
    int position;
    string removed;
    string added;

    Operation(string t, int p, string r, string a) {
        type = t;
        position = p;
        removed = r;
        added = a;
    }

    string apply(string text) {
        string before = text.substr(0, position);
        string after = text.substr(position + removed.length());
        return before + added + after;
    }

    string reverse(string text) {
        string before = text.substr(0, position);
        string after = text.substr(position + added.length());
        return before + removed + after;
    }

    // Shows a newline as \n so every history entry stays on ONE line
    string visible(string s) {
        string out = "";
        for (char c : s) {
            if (c == '\n') {
                out += "\\n";
            } else {
                out += c;
            }
        }
        return out;
    }

    string describe() {
        if (type == "INSERT") {
            return "INSERT \"" + visible(added) + "\" at position " + to_string(position);
        }
        if (type == "DELETE") {
            return "DELETE \"" + visible(removed) + "\" from position " + to_string(position);
        }
        return "REPLACE \"" + visible(removed) + "\" with \"" + visible(added) +
               "\" at position " + to_string(position);
    }
};