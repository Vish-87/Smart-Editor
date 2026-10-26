#pragma once
#include <string>
using namespace std;

class Operation{
    public:
    string type;
    int position;
    string removed;
    string added;


    Operation(string t, int p, string r, string a){
        type =t;
        position =p;
        removed = r;
        added =a;
    }

    string describe(){
        if(type == "INSERT"){
            return "INSERT \"" + added + "\" at position " + to_string(position);

        }
        else if(type == "DELETE"){
            return "DELETE \"" + removed + "\" at position " + to_string(position);
        }
        else if(type == "REPLACE"){
            return "REPLACE \"" + removed + "\" with \"" + added + "\" at position " + to_string(position);
        }
        else{
            return "Unknown operation";
        }
    }

    string apply(string text){
        string before = text.substr(0, position);
        string after = text.substr(position +removed.length());
        return before +added +after;

    }
    string reverse(string text){
        string before = text.substr(0, position);
        string after = text.substr(position +added.length());
        return before +removed +after;
    }
};