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
    if (strcmp(argv[0], "quit") == 0) /* is it an "quit"?  */
     	exit(0);                      /* exit if it is     */ 
    else if(strlen(argv[0]) == 0)     /* if argv[0] has no command, quit this function */
	return 0;
    else{
        pid_t pid = fork();           /* fork process      */
        if (pid == -1) {              /* error             */
	    char* error = strerror(errno);
	    printf("fork: %s\n", error);
	    return 1;
	}
        else if (pid == 0) {          /* child process     */
            execvp(argv[0], argv);    /* execute command   */
            char* error = strerror(errno); /* Error occurred */
            printf("\033[1;31merror:\033[1;94m %s \033[0;m: %s\n", argv[0], error);
	    exit(0);
	    return 0;
	    }
        else {                        /* parent process    */
            int childStatus;          /* wait for child process to finish */
            waitpid(pid, &childStatus, 0);
	    return 1;
	}
    }
}

void removeSpaces(char *str){
    int count = 0;		      /* To keep track of non-space character count */
    for (int i = 0; str[i]; i++)      /* Traverse the given string. */
	if (str[i] == ' ' && 	      /* skip spaces*/
           (str[i-1] == ' ' || str[i-1] == '\0'));      
        else str[count++] = str[i];   /* If current character is not space, */
				      /* then place it at index 'count++' */
    if(str[count-1] != ' ') 	      /* check if there is any ' ' behind string */
        str[count] = '\0';
    else
        str[count-1] ='\0';
}

char **splitCommand(char *line){
     char** split = (char **) malloc(80*sizeof(char *));
     char *token = strtok(line, ";");
     int i=0;
     while (token != NULL){
	 removeSpaces(token);         /* remove spaces from the token */   
	 split[i] = token;
         token = strtok(NULL, ";");
	 i++;
     }
     split[i] = NULL;
     return split;
}

void  main(int argc, char **argval){
     char  line[MAX_COMMAND_LENGTH + 1];     /* the input line             */
     char  *argv[MAX_NUMBER_OF_PARAMS + 1];  /* the command line argument  */
     char **split = (char **) malloc(20*sizeof(char *));

     FILE *file = fopen(argval[1], "r");
     int k = 0;
     if(argc > 1){
         if (file == 0)
             printf("Could not open file\n");
         else {
             char ch;
             char *str = (char *)malloc(20*sizeof(char*));
             int i = 0; 
             while  ((ch = fgetc(file)) != EOF ){
                 if (ch != '\n')
                     str[i++] = ch;	
		 else{
		     if(str[strlen(str)-1] == '\n')     
             	         str[strlen(str)-1] = '\0'; 
	             split = splitCommand(str);
                     int k = 0;
	             while(split[k] != NULL){
                         parse(split[k], argv); /* parse the line */         
	                 execute(argv);         /* otherwise, execute the command */
		         k++;
	             }         
                     memset(str,0,20*sizeof(char*));
		     i = 0;
		 }
	     }
	     fclose(file);
         }
     }
 
     while (argc == 1) {                      /* repeat until done ....  */
         printf("\033[1;93mshell -> ");       /* display a prompt        */
	 printf("\033[0;m");
	 
	 /* read command from standard input, exit on Ctrl+D */
         if(fgets(line, sizeof(line), stdin) == NULL) break;
         if(line[strlen(line)-1] == '\n')     /* remove trailing newline character */ 
             line[strlen(line)-1] = '\0'; 
	 split = splitCommand(line);
         int i = 0;
	 while(split[i] != NULL){
             parse(split[i], argv);           /* parse the line */
	     execute(argv);                   /* otherwise, execute the command */
             i++;
	 }
     }
}
