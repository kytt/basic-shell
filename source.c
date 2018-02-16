#include   <sys/types.h>
#include   <sys/wait.h>
#include   <unistd.h>
#include   <stdio.h>
#include   <stdlib.h>
#include   <string.h>

void  parse(char *line, char **argv)
{
     while (*line != '\0') {       /* if not the end of line ....... */ 
          while (*line == ' ' || *line == '\t' || *line == '\n')
               *line++ = '\0';     /* replace white spaces with 0    */
          *argv++ = line;          /* save the argument position     */
          while (*line != '\0' && *line != ' ' && 
                 *line != '\t' && *line != '\n') 
               line++;             /* skip the argument until ...    */
     }
     *argv = '\0';                 /* mark the end of argument list  */
}

void  execute(char **argv)
{
     pid_t  pid;
     int    status;

     if ((pid = fork()) < 0) {     /* fork a child process           */
          printf("*** ERROR: forking child process failed\n");
          exit(1);
     }
     else if (pid == 0) {          /* for the child process:         */
          if (execvp(*argv, argv) < 0) {     /* execute the command  */
               printf("%s: command not found\n", argv[0]);
               exit(1);
          }
     }
     else {                                  /* for the parent:      */
          while (wait(&status) != pid)       /* wait for completion  */
               ;
     }
}

char **splitCommand(char *line){
     char** split = (char **) malloc(20*sizeof(char *));
     char *token = strtok(line, ";");
     int i=0;
     while (token != NULL){	
         split[i] = token;
         token = strtok(NULL, ";");
	 i++;
     }
     split[i] = NULL;
     return split;
}

void  main(void)
{
     char  line[1024];             /* the input line                 */
     char  *argv[64];              /* the command line argument      */
     char** split = (char **) malloc(20*sizeof(char *));

     while (1) {                   /* repeat until done ....         */
          printf("\033[32;1mshell -> ");     /*   display a prompt             */
	  printf("\033[0;m");
	  scanf("%[^\n]",line);
	  getchar();
          //gets(line);              /*   read in the command line     */

	  split = splitCommand(line);

          int i = 0;
	  while(split[i] != NULL){
	      parse(split[i], argv);       /*   parse the line               */
	      if (strcmp(argv[0], "quit") == 0)  /* is it an "quit"?     */
	           exit(0);            /*   exit if it is                */
	      execute(argv);           /* otherwise, execute the command */
              i++;
	  }
     }
}
