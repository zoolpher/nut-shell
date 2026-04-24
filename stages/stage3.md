# Stage 3 - Command Execution

## What is this stage about?
This is the heart of the shell. Stage 3 takes the parsed `Command`
struct from Stage 2 and actually executes it. This is where our shell
stops being a fancy input reader and starts running real programs.

## How does command execution work?
Every command execution follows three steps:

**1. fork()** — creates a child process (a copy of the shell).
Returns different values depending on who reads it:
- `0` → we are in the child process
- `> 0` → we are in the parent, value is the child's PID
- `-1` → fork failed

**2. execvp()** — called by the child process. Replaces the child
with the actual program (e.g. `ls`, `python3`, `git`).
The `p` means it automatically searches `$PATH` to find the program —
we don't need to provide the full path like `/usr/bin/ls`.
Takes a C-style `char*` array, so we convert our `vector<string>` manually
and terminate it with `nullptr` so `execvp` knows where the array ends.

**3. waitpid()** — called by the parent. Blocks the shell until the
child finishes executing. Takes 3 arguments:
- `pid` → which child to wait for
- `nullptr` → we don't care about exit status for now
- `0` → default behavior, can use `WNOHANG` for background jobs later

## What we built in this stage?
- `executeCommand()` function that takes a `Command` struct
- `fork()` to create a child process
- `execvp()` to replace child with the real program
- `waitpid()` to make the shell wait for the child
- Empty args guard — if no command typed, return early

## Headers added
- `<unistd.h>` → for `fork()` and `execvp()`
- `<sys/wait.h>` → for `waitpid()`

## How to run?
```bash
make
./nut-shell
```
Test with:
```bash
ls -la
whoami
python3 --version
git status
echo hello world
```

## The REPL loop is now complete
```
Read → parseInput() → executeCommand() → loop
```
Every command you type is now parsed, forked, executed, and waited on. 🔥