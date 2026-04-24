# Stage 6 - Background Jobs

## What is this stage about?
Stage 6 implements background job execution using `&`. When a command
ends with `&`, the shell doesn't wait for it to finish — it returns
the prompt immediately while the child runs in the background.

## How do background jobs work?
When `&` is detected during parsing, `cmd.background` is set to `true`.
In `executeCommand`, the parent checks this flag:
- `background = false` → call `waitpid`, shell waits for child
- `background = true` → skip `waitpid`, shell returns immediately

The child process runs independently in the background.

## What is a zombie process?
When a child process finishes, it doesn't fully disappear. It stays
in the process table as a **zombie** — waiting for the parent to call
`waitpid` to collect its exit status.

For background jobs we skip `waitpid`, so finished children become
zombies and waste resources indefinitely. 🧟

## How does sigchldHandler fix this?
When any child process dies, the OS sends a `SIGCHLD` signal to the
parent. Our handler catches it and reaps all finished children:

```cpp
void sigchldHandler(int) {
    while (waitpid(-1, nullptr, WNOHANG) > 0);
}
```

- `waitpid(-1, ...)` → wait for ANY child
- `WNOHANG` → don't block, return immediately if no child is done
- `while` loop → reap ALL finished children in one shot

Registered once before the main loop with `signal(SIGCHLD, sigchldHandler)`.

## What we built in this stage?
- Background job support by skipping `waitpid` when `cmd.background = true`
- PID printed when background job starts
- `sigchldHandler` to prevent zombie processes
- `signal(SIGCHLD, sigchldHandler)` registered before the main loop

## How to run?
```bash
make
./nut-shell
```
Test with:
```bash
sleep 5 &
sleep 2 &
ls
```