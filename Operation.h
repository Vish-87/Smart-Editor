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