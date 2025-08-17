# **minishell**

## What is a shell? 🐚

Have you ever used the Command Prompt on Windows or the Terminal on a Mac? When you type a command like `ls` or `cd` and press Enter, there's a program running behind the scenes that understands what you typed and executes it. This program is called a "shell".

Our minishell project is a simplified version of a real shell. It's a command-line interface that allows us to type commands, run programs, and see the results, just like a regular shell, but built from scratch using **C**.

## Core Features
Our minishell project includes a number of features that mimic the behavior of a standard shell:

**Command Execution:** Run external programs and executables found in your system's `PATH`.

**Pipes (`|`):** Chain multiple commands together, where the output of one command becomes the input of the next.

**Redirections (`<`, `>`, `>>`):** Control where the input for a command comes from and where the output goes.

&emsp; `>`: Redirects a command's standard output to a file, overwriting the file's content.

&emsp; `>>`: Redirects a command's standard output to a file, appending to the file's content.

&emsp; `<`: Redirects a command's standard input from a file.

**Built-in Commands:** Our shell comes with its own implementation of several essential commands.

**Environment Variables:** Manage and use environment variables with commands like `echo $PATH` and `export`.

## Built-in Commands

We implemented our own versions of the following commands:

&emsp; `echo`: Displays text on the standard output.

&emsp; `cd`: Changes the current working directory.

&emsp; `pwd`: Prints the current working directory.

&emsp; `export`: Manages environment variables.

&emsp; `unset`: Removes environment variables.

&emsp; `env`: Prints the environment variables.

&emsp; `exit`: Exits the minishell, with an optional exit status.

## Installation

Clone the repo and run `make`, then launch the program `./minishell`.

## Contributors

[*Finnan Solomon*](https://github.com/finye)

[*Eetu Laine*](https://github.com/eetulaine)
