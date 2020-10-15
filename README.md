# Simple-Shell
1. Project #1 - Create Your own Shell
Project Instructions
The objective of this assignment is to understand the workings of a command-line interface (CLI) and to obtain a working knowledge of process forking and signaling.
Assignment:
In this project we will build a command shell in stages. Our command shell will be a simple implementation of the commonly used command shells such as sh, bash, csh, and tcsh.
The assignment is split-up in tasks to implement separate features of our shell program. The code fragments for these tasks does not need to be separately submitted. Only the final implementation should be submitted for grading. If you are not able to complete the final tasks, you should be able to submit a partial but working shell program.
The textbook discusses a simple shell implementation at the end of Chapter 3. You may use this information as a resource, as well as the labs that we have covered in class.
Project team:
You are allowed to work on this assignment in a team of at most   two  students; but not required.
Submission of source code and report:
Push your code to github, and post a github url in Google Classroom as your submission. You must include a report (two-page minimum) that describes your design choices and documentation of your code. Include your name and your teammate’s name (if you have a teammate) in all source codes and accompanying reports.
Requirements:
You need to be familiar with a UNIX command shell, such as bash, and you must be able to write, compile, and debug a C program.
Consult the manual pages or Google to understand the following system calls and library functions:
int chdir(const char *path)
int execvp(const char *file, char *const argv[])
void exit(int status)
pid_t fork(void)
char *getcwd(char *buf, size_t size)
char getenv(const char *name)
void perror(const char *string)
int setenv(const char *name, const char *value, int overwrite)
sig_t signal(int sig, sig_t func)
pid_t wait(int *status)
pid_t waitpid(pid_t wpid, int *status, int options)
Use the man command to access the manual pages. You also have Google!
Note that perror prints an error message when errno is set. It should only be used when a system call or library function call fails and errno is set.
Another useful function we will use is assert to assist us with debugging. For example:
assert(argc > 0);
checks if argc > 0. If the assertion fails, a diagnostic message is displayed. The error message points to the location in your program of the failed assertion. Whenever you have specific assumptions about the state of (your variables in) your program, it is a good idea to add an assertion.
Task 1 - Print the Shell Prompt and Adding Built-in Commands
We have created a file shell.c where you will develop a simple shell to execute commands from the command line. Your shell should prompt the user with current working directly. Use the getcwd() to get the current working directory and print a prompt using the following format <path>/<cur dir> followed by > (ex. /home/codio>). Your shell should interpret the following built-in commands:
cd: changes the current working directory
pwd: prints the current working directory
echo: prints a message and the values of environment variables
exit: terminates the shell
env: prints the current values of the environment variables
setenv: sets an environment variable
For example:
/home/codio> echo hello world
hello world
/home/codio> cd testDir1
/home/codio/testDir1> pwd
/home/codio/testDir1
/home/codio/testDir1> setenv greeting=hello
/home/codio/testDir1> echo $greeting $OSTYPE
hello linux-gnu
/home/codio/testDir1> env greeting
hello
/home/codio/testDir1> cd ..
/home/codio> pwd
/home/codio
/home/codio> env
...
PATH=/bin:/usr/bin:....
HOME=/home/codio
greeting=hello
...
/home/codio> exit
To implement the first task you will use the code already given in shell.c that gets input from the command line. Your job is to parse the command line into an array of tokens in order to execute the commands (first array element) and distinguish between parameters and flags (the rest of the array).
Your program must implement the built-in commands. Here are some helpful hints:
To implement $var variables, change the tokenize function to check for strings that start with a $. When a string starts with a $, use getenv to obtain the variable value and place it in the argument array. In this way, variables can be used with all commands, for example echo, cd, and setenv.
To implement echo, write a loop that runs over the command arguments to print the string values.
To implement env, you need to access to the global environment variables array. To this end, add the following declaration to your code: extern char **environ; (see manual pages for environ) For example, environ[0] contains the first variable name=value pair, environ[1] contains the second pair, and so on until you hit a terminating NULL string.
To implement setenv, use the setenv function with overwrite=1. You may assume that the assigned value is a single string token (i.e. the first argument is the variable name and the second argument is the value).
After testing your code, you can advance to task 2. You don’t need to submit your code at this point. Just make sure it works before you continue with task 2 by testing the example input above (we will test your code with different input).
Task 2 - Adding Processes
In this part of the assignment you will extend the shell program with process forking. The shell takes a command, checks if it is not a built-in command, forks a process, loads the program from the file system, passes the arguments, and executes it. The shell must wait for the child process to terminate and report any errors that might have occurred. Please reference Lab 2 to implement process forking.
Implement process forking in your shell.c program. Your shell program must be able to execute built-in commands and run executables from the command line.
For example:
/home/codio> ls
Makefile  README  ref_shell  shell.c  testData  testDir1  testDir2
/home/codio> cp shell.c shell2.c
/home/codio> ls
Makefile  README  ref_shell  shell2.c  shell.c  testData  testDir1  testDir2
/home/codio> abc
execvp() failed: No such file or directory
An error occurred.
/home/codio> ls -1
total 44
-rw-r--r-- 1 codio codio   130 Sep 25 05:28 Makefile
-rw-r--r-- 1 codio codio   927 Jan 30  2019 README
-rwxrwxr-x 1 codio codio 14395 Sep 25 08:02 ref_shell
-rw-rw-r-- 1 codio codio  8420 Sep 25 09:06 shell2.c
-rw-rw-r-- 1 codio codio  1570 Sep 25 08:42 shell.c
-rw-rw-r-- 1 codio codio   233 Sep 25 08:09 testData
drwxrwxr-x 1 codio codio     0 Sep 25 08:06 testDir1
drwxrwxr-x 1 codio codio     0 Sep 25 08:06 testDir2
/home/codio> exit
Task 3 - Adding Background Processes
Processes can be run on the background by appending a & at the end of a command line.
For example:
/home/codio> a.out &
/home/codio>
This starts the program a.out and the shell returns immediately to prompt for the next command. Implement background processes by checking (and removing) a & at the end of the command line.
Task 4 - Signal Handling
Your current shell implementation has a significant drawback: when we want to terminate a foreground process by depressing ctrl-C, the shell will quit as well. To prevent the shell from quitting, we need a signal handler to catch SIGINT (ctrl-C). Consult the manual page of signal and Lab 2.
For example:
/home/codio> a.out    
   .... out of a.out ...
   ....              ...
   ....              (type ctrl-C)
/home/codio>
When depressing ctrl-C, the shell should return to the prompt.
Task 5 - Killing off long running processes
Implement a timer that terminates a foreground process after 10 seconds have elapsed and the process has not completed. You should only terminate foreground processes that exceed the alloted time limit. To terminate a process, you can use int kill(pid_t, int sig). If the process completes before the timer expires, the shell should immediately return with the prompt.
Task 6 - Extra Credit (Double your grade!)
Your shell should allow for at least one of the following I/O redirection 1. using > or 2. using <, or 3. piping output from one process as input to another using |.
For example:
/home/codio> ls
Makefile  README  ref_shell  shell.c  testData  testDir1  testDir2
/home/codio> cat shell.c > outputfile 
/home/codio> ls
outuptfile  Makefile  README  ref_shell  shell.c  testData  testDir1  testDir2
/home/codio> more < shell.c
   .... show contents of shell.c ...
   .... on the screen pagenated  ...
   ....                          ...
/home/codio> cat testData | grep "this"
this line is the 1st lower case line in this file.
Two lines above this line is empty.
And this is the last line.
ref_shell - is an executable of the solution, for your reference. You can run ./ref_shell in the terminal to see what your shell should be doing.
