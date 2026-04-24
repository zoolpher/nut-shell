#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream>   // for istringstream
#include <vector>

using namespace std;

struct Command {
    vector<string> args;    // the command + its arguments
    string outFile;         // file for > or >>
    string inFile;          // file for <<
    bool append;            // >> or >?
    bool background;        // & ?
    // struct Command* nextCommand;    // for pipelines
};

Command parseInput(string input) {
    istringstream ss(input);
    string token;
    Command cmd;
    cmd.append = false;
    cmd.background = false;

    while(ss >> token) {
        // ss is the source, token is the destination. 
        // Every time the loop runs, token gets the next word. 
        
        
        if (token == ">") {
            ss >> token;            // get the next token which should be the output file
            cmd.outFile = token;   // that's the filename
        } 
        else if (token == ">>") {
            ss >> token;
            cmd.outFile = token;
            cmd.append = true;      // set append mode
        }
        else if (token == "<") {
            ss >> token;            // get the next token which should be the output file
            cmd.inFile = token;     // next token is inFile 
        }
        else if (token == "&") {
            cmd.background = true;  // set background flag
        } 
        // else if (token == "|") {
        //     // Handle pipeline: create a new Command for the next part of the pipeline
        //     cmd.nextCommand = new Command();
        //     cmd = *cmd.nextCommand; // Move to the next command in the pipeline
        // }
        else {
            cmd.args.push_back(token);
        }

    }
    return cmd;
}

int main() {
    while (true) {
        string userPrompt;
        cout<<"(\\) nut-shell-$ ";
        getline(cin, userPrompt);
        if (userPrompt == "exit") {
            break;
        }

        // Parse the user input into tokens.
        Command tokens = parseInput(userPrompt);
        Command cmd = parseInput(userPrompt);
        cout << "Command: " << cmd.args[0] << endl;
        cout << "outFile: " << cmd.outFile << endl;
        cout << "inFile: " << cmd.inFile << endl;
        cout << "background: " << cmd.background << endl;

    }
    return 0;
}