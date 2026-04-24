# Stage 4 - I/O Redirection

## What is this stage about?
Stage 4 makes redirection actually work. The parser already detects
`>`, `>>`, and `<` — now we make the shell honor them by redirecting
the child process's input/output to files using `dup2()`.

## What are file descriptors?
Every program gets 3 file descriptors automatically when it starts:
- **fd 0** → stdin (keyboard by default)
- **fd 1** → stdout (terminal by default)
- **fd 2** → stderr (terminal by default)

A file descriptor is just a number representing an open file or channel.
When you open a new file with `open()`, it gets the next available number
like fd 3, fd 4 etc.

## What is dup2()?
`dup2(newFd, oldFd)` replaces `oldFd` with `newFd`.

So `dup2(fd, 1)` means — replace stdout (fd 1) with our file.
Now anything the child writes to stdout goes to the file instead
of the terminal. The program (e.g. `ls`) doesn't know anything changed
— it still writes to fd 1. But fd 1 now points to the file. 🎯

## Why in the child process?
`dup2` must happen after `fork()` but before `execvp()` — inside the
child only. If we did it in the parent, the shell's own stdout would
get redirected, breaking the terminal permanently.

## Redirection modes
- `>` → `O_WRONLY | O_CREAT | O_TRUNC` — write, create if missing, wipe existing content
- `>>` → `O_WRONLY | O_CREAT | O_APPEND` — write, create if missing, append to existing content
- `<` → `O_RDONLY` — read from file, replace stdin (fd 0)

## What we built in this stage?
- Output redirection with `>` and `>>` using `dup2(fd, 1)`
- Input redirection with `<` using `dup2(fd, 0)`
- Used `cmd.append` flag to switch between `O_TRUNC` and `O_APPEND`
- Wrapped in empty checks so normal commands are unaffected

## How to run?
```bash
make
./nut-shell
```
Test with:
```bash
ls > output.txt
cat output.txt
ls >> output.txt
cat output.txt
cat < output.txt
```