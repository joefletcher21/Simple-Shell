#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#define MAX_COMMAND_LINE_LEN 1024
#define MAX_COMMAND_LINE_ARGS 128

char prompt[] = "> ";
char delimiters[] = " \t\r\n";
extern char **environ;

char *getcwd(char *buf, size_t size);

char cwd[100];
void sighandler(int);
//void siquit();
pid_t pid;

int main() {
    signal(SIGINT, sighandler);
    // Stores the string typed into the command line.
    char command_line[MAX_COMMAND_LINE_LEN];
    char cmd_bak[MAX_COMMAND_LINE_LEN];
  
    // Stores the tokenized command line input.
    char *arguments[MAX_COMMAND_LINE_ARGS];
    int i = 0;
    int j = 0;
    int k = 0;
    bool isBackground = false;
  
    while (true) {
      isBackground = false;
      
        do{ 
         
          
          
          if (getcwd(cwd, sizeof(cwd)) != NULL){
            //printf("%s>", cwd);
          }else{
            perror("getcwd() error");
            return 1;
          }
            // Print the current prompt.
            printf("%s>", cwd);
            fflush(stdout);

            // Read input from stdin and store it in command_line. If there's an
            // error, exit immediately. (If you want to learn more about this line,
            // you can Google "man fgets")
            if ((fgets(command_line, MAX_COMMAND_LINE_LEN, stdin) == NULL) && ferror(stdin)) {
                fprintf(stderr, "fgets error");
                exit(0);}
 
        }while(command_line[0] == 0x0A);  // while just ENTER pressed

      
        // If the user input was EOF (ctrl+d), exit the shell.
        if (feof(stdin)) {
            printf("\n");
            fflush(stdout);
            fflush(stderr);
            return 0;
        }

        // TODO: 
        
			  // 0. Modify the prompt to print the current working directory
			  
			
        // 1. Tokenize the command line input (split it on whitespace)

        arguments[0] = strtok(command_line, delimiters);
      
      
        i = 0;
        while(arguments[i] != NULL){
          arguments[++i] = strtok(NULL, delimiters);
        }
        arguments[i] == NULL;
        //for(j = 0; j<i; j++){
         // printf("%s\n",arguments[j]);
        //}
        // 2. Implement Built-In Commands
        
      
      
      
      
      
      
        if(strcmp(arguments[0], "cd")==0){
          
          chdir(arguments[1]);
        }
        else if(strcmp(arguments[0], "pwd")==0){
          if(getcwd(cwd, sizeof(cwd)) == NULL){
            perror("getcwd() error");
            return 1;
          }
          
          printf("%s\n", cwd);
          fflush(stdout);
        }
        else if (strcmp(arguments[0], "echo")==0){
          k = 1;
          while(arguments[k]!=NULL){
            printf("%s",arguments[k]);
            k++;
          }
          printf("\n"); 
          //printf("echo function\n");
        }
        else if (strcmp(arguments[0], "exit")==0){
          exit(0);
        }
        else if (strcmp(arguments[0], "env")==0){
          //getenv(arguments[1])
          printf("%s\n",getenv(arguments[1]));
        }
        else if (strcmp(arguments[0], "setenv")==0){
          
          printf("setenv function\n");
        }
        else {
          if (strcmp(arguments[i-1], "&") == 0){
            arguments[i-1] = NULL;
            isBackground = true;
          }
          
          
          pid = fork();
          if (pid < 0){
            perror("fork failed");
            exit(0);
          }
          else if (pid == 0){
            
            execvp(arguments[0], arguments);
            
            
            exit(0);
          }
          else{// 4. The parent process should wait for the child to complete unless its a background process
            
            if(isBackground != true){
              wait(NULL);
            }
          }
        }
        // 3. Create a child process which will execute the command line input
        // Hints (put these into Google):
        // man fork
        // man execvp
        // man wait
        // man strtok
        // man environ
        // man signals
        // Extra Credit
        // man dup2
        // man open
        // man pipes
    }
    // This should never be reached.
    return -1;
}


void sighandler(signum){
  printf("Caught signal %d, coming out...\n", signum);
  kill(pid, SIGKILL);
}
