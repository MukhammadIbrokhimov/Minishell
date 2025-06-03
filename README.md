# Minishell

Minishell is a simplified UNIX shell project developed as part of the 42 School curriculum. The goal is to understand how a shell works by creating a minimalistic version that can interpret and execute user commands.

---

## 🚀 Features

- Prompt display
- Execution of binary commands with arguments
- Built-in commands: `echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`
- Environment variables handling
- Pipes (`|`) and redirections (`>`, `>>`, `<`, `<<`)
- Signal handling (`CTRL+C`, `CTRL+\`)
- Quote management (`'`, `"`)
- Exit status handling
- Error messages similar to bash

---

## 🛠️ Built-ins Implemented

| Command | Description |
|---------|-------------|
| `echo`  | Display a line of text |
| `cd`    | Change the current directory |
| `pwd`   | Print the current working directory |
| `export`| Set environment variables |
| `unset` | Remove environment variables |
| `env`   | Show environment variables |
| `exit`  | Exit the shell |

---

## 📦 Compilation

```bash
make
```

## 🧠 What We Learned

- How shells parse and execute commands  
- Managing processes with `fork()`, `execve()`, and `wait()`  
- File descriptor and pipe management  
- Implementing a command parser  
- Handling environment and built-in commands  
- Signal handling and user input  

---

## 👥 Authors

- **Repo Owner**  
  [Mukhammad Ibrokhimov](https://github.com/mukhammadibrokhimov)

- **Teammate Name**  
  [Ghazaleh Ansari](https://github.com/ghazalehans)
