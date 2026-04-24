#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream>   // for istringstream
#include <vector>

#include <unistd.h>   // for fork and exec
#include <sys/wait.h> // for wait
#include <fcntl.h>    // for open

using namespace std;

struct Command {
    vector<string> args;    // the command + its arguments
    string outFile;         // file for > or >>
    string inFile;          // file for <
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


// ========================================================== STAGE 3 ==========================================================

void executeCommand(const Command& cmd) {
    if (cmd.args.empty()) return;

    pid_t pid = fork();    //fork() returns integer values 

    if (pid == 0) {
        // I am the CHILD → exec the command

        vector<char*> cargs;
        for (auto& arg : cmd.args) {
            cargs.push_back(const_cast<char*>(arg.c_str()));
        }

        cargs.push_back(nullptr); // must end with nullptr to determine the end of the arguments for execvp

        if (!cmd.outFile.empty()) {
            int flags = O_WRONLY | O_CREAT | (cmd.append ? O_APPEND : O_TRUNC);
            int fd = open(cmd.outFile.c_str(), flags, 0644);
            dup2(fd, 1);
            close(fd);
        }
        if (!cmd.inFile.empty()) {
            int fd = open(cmd.inFile.c_str(), O_RDONLY);
            dup2(fd, 0);  // replace stdin
            close(fd);
        }

        execvp(cargs[0], cargs.data());
    }

    else if (pid > 0) {
        // I am the PARENT → wait for child
        waitpid(pid, nullptr, 0); // wait for the child process to finish
    }

    else {
        // fork failed
        cerr << "Error: fork failed" << endl;
    }
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
        Command cmd = parseInput(userPrompt);
        // cout << "Command: " << cmd.args[0] << endl;
        // cout << "outFile: " << cmd.outFile << endl;
        // cout << "inFile: " << cmd.inFile << endl;
        // cout << "background: " << cmd.background << endl;

        executeCommand(cmd);

    }
    return 0;
}