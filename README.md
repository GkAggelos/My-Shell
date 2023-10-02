# My-Shell
Simple shell for Unix. To compile run "make" and then "./mysh" to execute.

## Parsing
The 3 functions for parsing is **main()**, **processString()** and **execute_command()**.

**`Main`** reads the command saves it for history purpose and calls the function **execute_command()**, also 
defines a function handler for SIGCHLD signal and ignores ctrl c and ctrl z signals.

**`Execute_command`** parses the command that takes as argument and if the command is one of those that 
Unix has no defult implementation exetuces it directly or calls the function **processString()** to handle it.

**`ProcessString`** implements the basics features of the shell (such as Redirections, Pipes, Background 
commands and Wild characters) and executes program files if there are any.

## 1. Redirections

**ProcessString()** is responsible for the Redirections. If the command has a redirection token (<, >, >>) the
next token has to be a file name that has to be saved, so the next child that will execute the program file knows 
where to save the output or to take the input. Also saves the type of the redirection so the child knows how to handle it.     

## 2. Pipes

**ProcessString()** is responsible for the Pipes. If the command has a pipe token ( | ) then has to genarate a pipe.
In this case the child will execute the command that have been read until the pipe token occurred and the parent
will call the **ProcessString()** again to execute the next part of the commnand. Both child and parent knows the
read end and write end of the pipe, so the child send the output to the parent and the parent takes the input from
the child.

## 3. Background commands

**ProcessString()** is responsible for the Background commands. If the command has a background token (&) then
the child ignores ctrl c and ctrl z signals. On the other hand the parent does not wait for the child and 
continues with the next command, until a SIGCHLD signal occurres and waits for the child status so we do not have
zombies.

## 4. Wild characters

**ProcessString()** is responsible for the Wild characters. If a token has a wildcard (*, [, ?) then it calls the
**wildcard()** function that with the help of **glob()** returns the file subset or an error message. 

## 5. Aliases

Aliases is variables of type struct aliasEntry and are saved in a table. **Execute_command** has the implementation
of all the commands for aliases (alias execution, save alias and delete alias). When have to execute an alias just
replace the alias with the corresponding command. 

## 6. Signals

The shell ignores ctrl c and ctrl z signals and restores the defult actions for a program execution that is not runing
at the background. Also the function **background_termination()** handles the SIGCHLD so the background processes will
never become zombies.

## 7. History 

The history is saved in a table and shows up with the command "history". With command "history [command number]" the
command with this number is executed.

## General

I have implement and the command cd (is at the basics commands that **execute_command()** executes).

When a child do not return a status 0 i print the id and the status of the child.

The command "printalias" prints all the aliases.

There is a chance that when a background process prints its output the shell promt prints with it.

All the tokens of the command has to be separated for example ./count1&; ./count2&; will not be able 
to execute but ./count1 &; ./count2 &; will be.     
