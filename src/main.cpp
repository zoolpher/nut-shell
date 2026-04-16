#include <stdio.h>
#include <string>
#include <iostream>
using namespace std;

int main() {
    while (true) {
        string userPrompt;
        cout<<"(\\) nut-shell-$ ";
        getline(cin, userPrompt);
        if (userPrompt == "exit") {
            break;
        }
        cout<<userPrompt<<endl;

    }
    return 0;
}