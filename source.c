#include   <sys/types.h>
#include   <sys/wait.h>
#include   <unistd.h>
#include   <stdio.h>
#include   <stdlib.h>
#include   <string.h>
#include   <errno.h>

#define MAX_COMMAND_LENGTH 100
#define MAX_NUMBER_OF_PARAMS 10

void  parse(char *line, char **argv){
    for(int i = 0; i < MAX_NUMBER_OF_PARAMS; i++) {
        argv[i] = strsep(&line, " ");
        if(argv[i] == NULL) break;
    }
}

int  execute(char **argv){
    // Fork process
    pid_t pid = fork();

    // Error
    if (pid == -1) {
        char* error = strerror(errno);
        printf("fork: %s\n", error);
        return 1;
    }

    // Child process
    else if (pid == 0) {
        // Execute command
        execvp(argv[0], argv);  

        // Error occurred
        char* error = strerror(errno);
        printf("\033[1;31merror:\033[1;94m %s \033[0;m: %s\n", argv[0], error);
        return 0;
    }

    // Parent process
    else {
        // Wait for child process to finish
        int childStatus;
        waitpid(pid, &childStatus, 0);
        return 1;
    }
}

void removeSpaces(char *str){
    int count = 0;		    /* To keep track of non-space character count */
    for (int i = 0; str[i]; i++)    /* Traverse the given string. */
	if (str[i] == ' ' && 	    /* skip spaces*/
           (str[i-1] == ' ' || str[i-1] == '\0'));      
        else str[count++] = str[i]; /* If current character is not space, */
				    /* then place it at index 'count++' */
    str[count] = '\0';
}

char **splitCommand(char *line){
     char** split = (char **) malloc(80*sizeof(char *));
     char *token = strtok(line, ";");
     int i=0;

     while (token != NULL){
	 removeSpaces(token);             /* remove spaces from the token*/
	 if (strcmp(token, "quit") == 0)  /* is it an "quit"?  */
	     exit(0);                     /* exit if it is     */
	 
	 if(token != '\0') 
	     //printf("%s\n", token);        
	     split[i] = token;
         token = strtok(NULL, ";");
	 i++;
     }
     split[i] = NULL;

     return split;
}

void  main(void)
{
     char  line[MAX_COMMAND_LENGTH + 1];     /* the input line                 */
     char  *argv[MAX_NUMBER_OF_PARAMS + 1];  /* the command line argument      */
     char** split = (char **) malloc(20*sizeof(char *));
     int cmdCount = 0;
     
     while (1) {                             /* repeat until done ....         */
          printf("\033[1;93mshell -> ");     /*   display a prompt             */
	  printf("\033[0;m");
	  
	  /* read command from standard input, exit on Ctrl+D */
          if(fgets(line, sizeof(line), stdin) == NULL) break;

          if(line[strlen(line)-1] == '\n') { /* remove trailing newline character */ 
              line[strlen(line)-1] = '\0'; 
          } 

	  split = splitCommand(line);
          int i = 0;

	  while(split[i] != NULL){
              parse(split[i], argv);    /*   parse the line */
	      execute(argv);           /* otherwise, execute the command */
              i++;
	  }
     }
}
