/*Source file for functions used in main*/
/*Dawid Tkaczyk 19386321  dawid.tkaczyk@ucdconnect.ie*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#include "functions.h"	//Header file

void changeDir(char **output){		//Changes the directory of the shell using system call chdir
        if(output[1] == NULL){			//if no directory specified set to home
                chdir(getenv("HOME"));		//getenv gets path for home environment variable
        }
        else{					//Else set to directory specified
                chdir(output[1]);	
                perror("Error: ");		//Prints last error that occured if system call fails
        }
}

void getTime(char timeString[14]){	//Gets current time and formats its into a string of format below
        time_t t;			
        t = time(NULL);			//gets current time
        struct tm *currentTime;		
        currentTime = localtime(&t);	//converts current time to correct timezone 
        strftime(timeString, 14, "[%d/%m %H:%M]", currentTime);	//formats current time into string
}

void changeOut(char *filepath){		//Changes stdout path
        int f = open(filepath,O_WRONLY|O_CREAT|O_TRUNC,0666);//Opens given path, gets file descriptor
        dup2(f, 1); //sets stdout to file
}

