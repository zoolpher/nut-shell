# nut-shell 🔩

A Unix shell built from scratch in C++ — supports command execution, I/O redirection, pipes, and background jobs.

Built on WSL/Linux using POSIX APIs (`fork`, `execvp`, `waitpid`, `dup2`, `pipe`).

---

## Features

- **REPL Loop** — interactive prompt that reads, evaluates, and loops forever
- **Command Parsing** — tokenizes input and detects special characters (`>`, `>>`, `<`, `|`, `&`)
- **Command Execution** — runs real programs via `fork` + `execvp` + `waitpid`
- **I/O Redirection** — redirect output (`>`, `>>`) and input (`<`) to/from files
- **Pipes** — connect commands with `|` (e.g. `ls | grep txt`)
- **Background Jobs** — run commands in background with `&`, zombie prevention via `SIGCHLD`

---

## How to build and run

```bash
git clone https://github.com/zoolpher/nut-shell.git
cd nut-shell
make
./nut-shell
```

---

## Usage examples

```bash
(\) nut-shell-$ ls -la
(\) nut-shell-$ echo hello world
(\) nut-shell-$ ls > output.txt
(\) nut-shell-$ cat < output.txt
(\) nut-shell-$ ls >> output.txt
(\) nut-shell-$ ls | grep txt
(\) nut-shell-$ sleep 5 &
(\) nut-shell-$ exit
```

---

## Project structure

```
nut-shell/
├── src/
│   └── main.cpp        # all source code
├── include/            # headers (future use)
├── stages/
│   ├── stage1.md       # REPL Loop
│   ├── stage2.md       # Command Parsing
│   ├── stage3.md       # Command Execution
│   ├── stage4.md       # I/O Redirection
│   ├── stage5.md       # Pipes
│   └── stage6.md       # Background Jobs
├── Makefile
└── README.md
```

---

## Stages

| Stage | Feature | Description |
|-------|---------|-------------|
| 1 | REPL Loop | Prompt, read input, loop forever, exit cleanly |
| 2 | Command Parsing | Tokenize input, detect `>` `>>` `<` `&` `\|` |
| 3 | Command Execution | `fork` + `execvp` + `waitpid` |
| 4 | I/O Redirection | `dup2` to redirect stdin/stdout to files |
| 5 | Pipes | Connect processes with `pipe` + `dup2` |
| 6 | Background Jobs | Skip `waitpid` for `&`, reap zombies with `SIGCHLD` |

---

## Stage 7 — Additional feat. (Coming Soon)

- [ ] Multi-pipe support (3+ commands in a pipeline)
- [ ] Quote handling (`"hello world"` as single token)
- [ ] Background job completion message
- [ ] Built-in commands (`cd`, `pwd`) 
- [ ] Error messages for unknown commands
- [ ] Command history (↑ arrow)

---

## System calls used

| Call | Purpose |
|------|---------|
| `fork()` | Create child process |
| `execvp()` | Replace child with program, searches `$PATH` |
| `waitpid()` | Parent waits for child to finish |
| `dup2()` | Redirect file descriptors |
| `pipe()` | Create pipe between two processes |
| `signal()` | Register SIGCHLD handler for zombie prevention |

---

## Author

[zoolpher](https://github.com/zoolpher/nut-shell) — B.Tech CS, building toward FAANG 🚀