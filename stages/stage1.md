# Stage 1 - REPL Loop

## What is this stage about?
This is the foundation of nut-shell. Before any command execution,
parsing, or special characters — the shell needs to simply exist.
Stage 1 builds the bare minimum: a loop that prints a prompt, reads
user input, echoes it back, and exits cleanly when the user types `exit`.

## What is a REPL?
REPL stands for Read → Evaluate → Print → Loop.
- **Read** — take full line of input from the user via `getline()`
- **Evaluate** — check if input is `exit`, otherwise process it
- **Print** — echo back what the user typed (execution comes in Stage 3)
- **Loop** — go back to step 1, repeat forever until user types `exit`

## How does it work?
The shell prints a prompt `(\) nut-shell-$`, waits for the user to type
something using `getline()`, reads the entire line including spaces,
echoes it back, and loops. If the user types `exit` the loop breaks
and the program terminates cleanly.

## What we built in this stage?
- A prompt that displays `(\) nut-shell-$`
- A `while(true)` loop that continuously reads user input
- `getline()` to capture full input including spaces
- Exits cleanly when user types `exit`

## How to run?
```bash
make
./nutshell
```

## Final code
```cpp
#include <stdio.h>
#include <string>
#include <iostream>
using namespace std;

int main() {
    while (true) {
        string userPrompt;
        cout << "(\) nut-shell-$ ";
        getline(cin, userPrompt);
        if (userPrompt == "exit") {
            break;
        }
        cout << userPrompt << endl;
    }
    return 0;
}
```