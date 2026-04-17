# Minishell

[![build](https://github.com/MukhammadIbrokhimov/Minishell/actions/workflows/build.yml/badge.svg)](https://github.com/MukhammadIbrokhimov/Minishell/actions/workflows/build.yml)

A UNIX shell reimplementation in C. Parses and executes commands with pipes, redirections, heredocs, environment-variable expansion, quote handling, signal handling, and 7 built-in commands — all within the [42 norm](https://github.com/42School/norminette) (80-char lines, 25-line functions, no globals except a signal flag).

Built with [Ghazaleh Ansari](https://github.com/ghazalehans) as part of the 42 Berlin Common Core.

## Features

- Interactive prompt powered by GNU Readline, with history
- Command execution via `fork` + `execve` with `PATH` resolution
- Pipes (`|`) and combinations thereof
- I/O redirections: `<`, `>`, `>>`, `<<` (heredoc)
- Environment variable expansion (`$VAR`, `$?`) inside and across quote contexts
- Single-quote and double-quote handling with correct expansion semantics
- Signal handling: `Ctrl-C` (SIGINT), `Ctrl-\` (SIGQUIT), `Ctrl-D` (EOF)
- Exit-status propagation compatible with `bash`'s behaviour
- Built-ins: `echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`

## Architecture

Input flows through three stages:

1. **Tokenizer** (`srcs/parsing/token_utils*.c`) — splits the raw line into tokens respecting quotes and shell meta-characters (`| & ; < > ( )`).
2. **Parser** (`srcs/parsing/parse*.c`) — builds a tree of command nodes. Node kinds: `EXEC`, `REDIR`, `PIPE`, `LIST`, `BACK`, `HEREDOC`.
3. **Executor** (`srcs/execution/*.c`) — walks the tree, setting up file descriptors and forking processes as needed. Heredocs are materialised via temporary pipes before the pipeline starts.

Memory ownership is traced through every exit path — including signal-driven interrupts during heredoc input — which was the hardest part of the project.

## Build and run

Minishell depends on GNU Readline.

```bash
# Debian/Ubuntu
sudo apt-get install libreadline-dev

# macOS (Apple Silicon Homebrew)
brew install readline
# Makefile flags may need adjustment if readline isn't at /opt/homebrew

make
./minishell
```

The vendored `includes/libft/` provides the 42 libft (strings, memory, linked list, ft_printf) used throughout the project.

## Constraints

From the 42 subject:

- C, compiled with `cc -Wall -Wextra -Werror`
- No leaks (verified with `valgrind --leak-check=full`)
- Limited set of permitted libc calls (`readline`, `printf`, `malloc`, `free`, `write`, `access`, `open`, `read`, `close`, `fork`, `wait`/`waitpid`/`wait3`/`wait4`, `signal`, `sigaction`, `sigemptyset`, `sigaddset`, `kill`, `exit`, `getcwd`, `chdir`, `stat`/`lstat`/`fstat`, `unlink`, `execve`, `dup`/`dup2`, `pipe`, `opendir`/`readdir`/`closedir`, `strerror`, `perror`, `isatty`, `ttyname`, `ttyslot`, `ioctl`, `getenv`, `tcsetattr`/`tcgetattr`, `tgetent`/`tgetflag`/`tgetnum`/`tgetstr`/`tgoto`/`tputs`)
- 42 norm: 80-char lines, ≤25-line functions, no globals except one `volatile sig_atomic_t` for the signal flag

## What was technically hard

- **Heredoc with expansion**: collecting the heredoc body while still respecting `$VAR` expansion (unless the delimiter was quoted) and handling `Ctrl-C` cleanly during collection.
- **Quote-aware tokenisation**: `echo "$USER's home"` must expand `$USER` but treat the embedded apostrophe as a literal.
- **Signal handling across `fork`**: the child and parent need different `SIGINT` behaviour; the executor installs/restores handlers at the right moments.
- **No-leak discipline**: every allocation traced through normal, error, and signal-interrupt paths.

## Authors

- [Mukhammad Ibrokhimov](https://github.com/MukhammadIbrokhimov)
- [Ghazaleh Ansari](https://github.com/ghazalehans)
