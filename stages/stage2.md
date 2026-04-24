# Stage 2 - Command Parsing

## What is this stage about?
Before the shell can execute anything, it needs to understand what the
user typed. Stage 2 builds the parser — it reads the raw input string,
splits it into tokens, detects special characters, and packages
everything into a clean `Command` struct for the next stage to use.

## What is parsing?
Parsing means reading input and extracting meaning from it.
For a shell, parsing means taking a raw string like `ls -la > output.txt`
and figuring out:
- What is the command? → `ls`
- What are the arguments? → `-la`
- Is there redirection? → yes, `>` to `output.txt`
- Is it a background job? → no

## What is istringstream?
`istringstream` treats a string like a stream, allowing us to extract
words one by one using the `>>` operator — the same way `cin >>` reads
from the keyboard. It automatically splits on spaces, making it perfect
for tokenizing user input.

## What is the Command struct?
The `Command` struct groups all parsed information into one clean unit:
- `args` → the command and its arguments e.g. `["ls", "-la"]`
- `outFile` → output file for `>` or `>>`
- `inFile` → input file for `<`
- `append` → true if `>>`, false if `>`
- `background` → true if `&` is present

We use a struct instead of separate variables because all these fields
belong to the same command — grouping them makes the code cleaner and
easier to pass around between functions.

## What we built in this stage?
- `parseInput()` function that takes raw input and returns a `Command`
- Detection of `>`, `>>`, `<`, `&` special characters
- All normal tokens pushed into `cmd.args`
- Special character targets captured correctly

## How to run?
```bash
make
./nutshell
```
Test with:
```bash
ls -la > output.txt
sleep 10 &
cat < input.txt
```