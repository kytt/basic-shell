#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<signal.h>
#include<string.h>

#define STR_LEN 100

char **splitSpace(char str[]){
	char** split = (char **) malloc(20 * sizeof(char *));
	// Returns first token 
    	char *token = strtok(str, " ");
   	int i=0;
    	// delimiters present in str[].
    	while (token != NULL)
    	{	
		split[i] = token;
		
       		//printf("%s1", split[i]);
        	token = strtok(NULL, " ");
		i++;
    	}
	split[i++] = NULL;
	return split;
}
void execute(char **cmd){
	pid_t pid = fork();
	if(!pid) {
		if( execvp(cmd[0],cmd) >0){
			execvp(cmd[0],cmd);
			exit(0);
		}
		else
			printf("\"%s\" command not found! \n",cmd);
	}
	else
		wait();
}

int main(int argc, char *argv[])
{	
	char str[STR_LEN];
	char **cmd;
	while(1){

		printf("prompt > ");
		gets(str);
		cmd = splitSpace(str);
		if(!strcmp(cmd[0],"exit"))
			break;
		execute(cmd);
		
	}
}
