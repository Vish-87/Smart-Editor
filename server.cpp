#include <iostream>
#include <string>
#include <mutex>
#include <cstdlib>
#include "httplib.h"
#include "Commands.h"
using namespace std;

TextEditor editor;   // one editor shared by everyone who opens the page
mutex editorLock;    // makes sure two requests never change it at the same time

// Make a string safe to put inside JSON "quotes"
string escapeJson(const string &s) {
    string out = "";
    for (char c : s) {
        if (c == '"')       out += "\\\"";
        else if (c == '\\') out += "\\\\";
        else if (c == '\n') out += "\\n";
        else if (c == '\r') out += "\\r";
        else if (c == '\t') out += "\\t";
        else                out += c;
    }
    return out;
}

// Build the JSON reply the website expects
string buildJson(const string &message) {
    string json = "{";
    json += "\"text\":\"" + escapeJson(editor.getText()) + "\",";
    json += "\"message\":\"" + escapeJson(message) + "\",";
    json += "\"history\":\"" + escapeJson(editor.showHistory()) + "\",";
    json += string("\"canUndo\":") + (editor.canUndo() ? "true" : "false") + ",";
    json += string("\"canRedo\":") + (editor.canRedo() ? "true" : "false");
    json += "}";
    return json;
}

int main() {
    httplib::Server svr;

    // Online hosts tell us which port to use through the PORT variable.
    // On your own computer there is no PORT, so we use 8080 and only allow
    // connections from your own machine.
    int port = 8080;
    string host = "localhost";
    const char* envPort = getenv("PORT");
    if (envPort != nullptr) {
        port = atoi(envPort);
        host = "0.0.0.0";   // accept connections from the internet
    }

    // Serve the website files from the "frontend" folder
        if (!svr.set_mount_point("/", "./Frontend")) {
        cout << "ERROR: could not find the 'Frontend' folder here." << endl;
        cout << "Run the server from the folder that contains 'Frontend'." << endl;
        return 1;
    }

    // The page sends a command here
    svr.Post("/api/command", [](const httplib::Request &req, httplib::Response &res) {
        lock_guard<mutex> guard(editorLock);
        string message = handleCommand(editor, req.body);
        res.set_content(buildJson(message), "application/json");
    });

    // The page asks for the current state when it first loads
    svr.Get("/api/state", [](const httplib::Request &, httplib::Response &res) {
        lock_guard<mutex> guard(editorLock);
        res.set_content(buildJson(""), "application/json");
    });

    cout << "Server running on port " << port << "." << endl;
    cout << "Local: open http://localhost:" << port << " in your browser." << endl;
    cout << "Press Ctrl+C to stop." << endl;

    if (!svr.listen(host.c_str(), port)) {
        cout << "Could not start the server. Is port " << port << " already in use?" << endl;
        return 1;
    }
    return 0;
}