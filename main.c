/*Implementation of a simple shell as described in assignment 2
 * created by Dawid Tkaczyk, 19386321, dawid.tkaczyk@ucdconnect.ie
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#include "functions.h"	//Header file

static void sigInt_handler(int sigNum){		//SIGINT handler for shell
        if(sigNum == SIGINT){				//if SIGINT signal
                char timeString[14];			//time string for prompt
                getTime(timeString);			//fills time string with current time
                printf("\n%s# ", timeString);		//prints prompt
                fflush(stdout);				//flushes out everything typed before signal
        }
}

int main(int argc, char **argv){

        while(1){	//Infinite loop (until CNTRL + D)
                if(signal(SIGINT, sigInt_handler) == SIG_ERR){
                        printf("Cant catch signal\n");
                }	//guard + sets signal handler for SIGINT

                char *input = NULL;
                size_t input_size = 0;
                char timeString[14];

                getTime(timeString);		//Gets current time
                printf("%s# ", timeString);	//prints prompt

                getline(&input, &input_size, stdin);	//Gets input
                input[strcspn(input, "\n")] = '\0';	//Removes trailing newline char

                char inputcpy[100];
                int fileFlag = 0;	//flag if stdout should be changed
                char *file;
                if(strstr(input, ">")!=NULL){	//If file specifier present
                        fileFlag = 1;		
                        strcpy(inputcpy, input);	//copies input
                        input[(int)(strchr(input, '>')-input)] = '\0';	//removes file specifier and path from input
                        
			//Sets file path !!!!!DO NOT INPUT SPACE AFTER '>' CHARACTER!!!!!
			file = strtok(inputcpy, ">");			
                        file = strtok(NULL, ">");
                }
                char **output =  malloc(sizeof(char*));	//Sets starting size to 1 address
                int argCount = 0;

                char *itt = strtok(input, " ");		//Gets first token
                while(itt != NULL){
                        output[argCount] = itt;		//Sets address to token
                        argCount++;
                        output = realloc(output, (argCount+1)*sizeof(char*));	//Increases size by 1 address
                        itt = strtok(NULL, " ");	//Gets next token
                }	
                output[argCount] = NULL;	//Null terminates list or arguments

                int childPid = fork();	//Creates child
                if(childPid == 0){
                        if(fileFlag != 0){	//if file specified
                                changeOut(file);	//Change stdout (only for child process)
                        }
                        if(strcmp(output[0], "cd") == 0){	//If cd command
                                changeDir(output);		//Change directory
                        }
                        else{
                                sleep(1);
                                execvp(output[0], output);	//execute command
                                printf("Error: Unkown Command\n");	//if command fails
                                exit(0);
                        }
                }
                else{
                        signal(SIGINT, SIG_IGN); //set interupt signal to not affect parent while child is running
                        wait(NULL);		 //wait for child
                        signal(SIGINT, sigInt_handler);	//reset interupt signal
                }
        }
}

