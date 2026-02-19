# minishell

`minishell` is a small Unix-like shell implemented in C as part of the Hive Helsinki curriculum.  
It re-implements a subset of `bash` behaviour: reading user input, parsing commands (including quotes
and environment variables), handling pipes and redirections, running external programs and
supporting a set of built-in commands.

## Features

- Interactive prompt with command history
- Execution of external programs with arguments
- Environment variable support (`$VAR` expansion)
- Quoting support (single and double quotes)
- Pipes (`cmd1 | cmd2 | cmd3`)
- Input/output redirections (`>`, `>>`, `<`)
- Basic signal handling (e.g. `Ctrl-C`)
- Built-in commands:
  - `cd`
  - `echo`
  - `pwd`
  - `export`
  - `unset`
  - `env`
  - `exit`

*(If some of these are not implemented or behave differently, edit this list accordingly.)*

## How to build and run

Requirements:

- `gcc` or compatible C compiler
- `make`
- Linux environment
- (Optional) `readline` library, if your implementation uses it

Build:

```bash
make
```
Run:
```bash
./minishell
```
To exit the shell, type:
```bash
exit
```
or use Ctrl-D on an empty line.

## Technologies used
C (C99)

POSIX APIs (processes, file descriptors, signals)

Linux

Makefile build system

(Optional) GNU Readline for line editing and history

## My contributions
This project was implemented as a team of two students at Hive Helsinki.

My main responsibilities were:

- Parsing and tokenisation of the input line

  - Handling of quotes (single and double)

  - Splitting into commands, arguments and operators (pipes, redirections)

  - Environment variable expansion

- Parts of the execution layer:

  - Handling exit statuses and errors

- Memory management in the parser and executor:

  - Designing the data structures

  - Cleaning up allocated memory for each command line

In addition, I participated in debugging, Valgrind analysis and general refactoring across the codebase.

