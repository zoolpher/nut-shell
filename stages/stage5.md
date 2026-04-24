# Stage 5 - Pipes

## What is this stage about?
Stage 5 implements pipes — the `|` operator that connects the output
of one command to the input of another. For example `ls | grep txt`
sends the output of `ls` directly as input to `grep`.

## How do pipes work?
A pipe is a one-way communication channel between two processes:
- `pipefd[0]` → read end (input)
- `pipefd[1]` → write end (output)

Anything written to `pipefd[1]` can be read from `pipefd[0]`.

## The pipeline execution flow
For `ls | grep txt`:
1. `pipe(pipefd)` → create the pipe
2. Fork **child 1** (ls) → redirect stdout to `pipefd[1]`, exec `ls`
3. Fork **child 2** (grep) → redirect stdin to `pipefd[0]`, exec `grep`
4. Parent closes both ends, waits for both children

## Why does the parent close both pipe ends?
If the parent keeps `pipefd[1]` open, the second command (`grep`) waits
forever for more data — the write end is still open so it never gets
EOF. Closing both ends in the parent ensures `grep` gets EOF when `ls`
finishes, and prevents file descriptor leaks.

## Why parsePipeline instead of parseInput?
`parseInput` returns a single `Command`. For pipes we need multiple
commands. `parsePipeline` splits the input by `|` using `getline`,
calls `parseInput` on each segment, and returns a `vector<Command>`.

## Why not call executeCommand inside executePipeline?
`executeCommand` calls `fork()` internally. We're already inside a
child process — calling `fork()` again would create unwanted extra
processes. We call `execvp` directly instead.

## Current limitation
Our implementation supports one pipe (two commands) only.
Multi-pipe support (3+ commands) is a Stage 7 bonus feature.

## What we built in this stage?
- `parsePipeline()` — splits input by `|` into `vector<Command>`
- `executePipeline()` — forks two children, connects them with a pipe
- Updated `main()` to route single vs pipeline commands correctly

## How to run?
```bash
make
./nut-shell
```
Test with:
```bash
ls | grep txt
echo hello world | grep hello
cat output.txt | grep LICENSE
```