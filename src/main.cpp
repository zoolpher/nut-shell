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




// ========================================================== STAGE 2 ==========================================================

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



// ========================================================== STAGE 5 ==========================================================
/*
Creates pipelines by splitting the input on the '|' character and parsing each segment as a separate command.
*/

vector<Command> parsePipeline(string input) {
    vector<Command> pipeline;
    istringstream ss(input);
    string segment;
    
    while (getline(ss, segment, '|')) {
        pipeline.push_back(parseInput(segment));
    }
    
    return pipeline;
}

void executePipeline(vector<Command>& pipeline) {
    int pipefd[2];
    pipe(pipefd); // create the pipe

    // First child - runs left command (ls)
    pid_t pid1 = fork();
    if (pid1 == 0) {
        dup2(pipefd[1], 1); // redirect stdout to write end
        close(pipefd[0]);   // close unused read end
        close(pipefd[1]);   // close original write end
        
        // exec the first command
        vector<char*> cargs;
        for (auto& arg : pipeline[0].args) {
            cargs.push_back(const_cast<char*>(arg.c_str()));
        }
        cargs.push_back(nullptr);
        execvp(cargs[0], cargs.data());

    }

    // Second child - runs right command (grep)
    pid_t pid2 = fork();
    if (pid2 == 0) {
        dup2(pipefd[0], 0); // redirect stdin to read end
        close(pipefd[1]);   // close unused write end
        close(pipefd[0]);   // close original read end
        
        // exec the second command
        vector<char*> cargs;
        for (auto& arg : pipeline[1].args) {
            cargs.push_back(const_cast<char*>(arg.c_str()));
        }
        cargs.push_back(nullptr);
        execvp(cargs[0], cargs.data());
    }

    // Parent
    close(pipefd[0]);
    close(pipefd[1]);
    waitpid(pid1, nullptr, 0);
    waitpid(pid2, nullptr, 0);
}




int main() {
    while (true) {
        string userPrompt;
        cout<<"(\\) nut-shell-$ ";
        getline(cin, userPrompt);
        if (userPrompt == "exit") {
            break;
        }

        
        vector<Command> pipeline = parsePipeline(userPrompt);
        if (pipeline.size() == 1) {
            executeCommand(pipeline[0]); // single command, no pipe
        } else {
            executePipeline(pipeline); // multiple commands, use pipes
        }

    }
    return 0;
}