// /**************************************************************
// * Class:  CSC-415-01 Fall 2022
// * Name: Jiayi Gu
// * Student ID : 920024739
// * GitHub ID : Jiayi079
// * Project: Assignment 3 – Simple Shell
// *
// * File: <Gu_Jiayi_HW3_main.c>
// *
// * Description:
// *    
// *    Creating a simple shell that let the user input and execute
// *    the arguement command line.
// *
// **************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>


void main()
{
    // maximum buffer size - 123
    int buffer_size = 123;
    char userInput[buffer_size];
    char * argv[buffer_size / 2];  // pointer should be the half size
    int keep = 0;

    while (1) {
        printf("prompt$ ");

        // read in the command line
        if (fgets(userInput, buffer_size, stdin) == NULL)
        {
            // if fgets null which means end of the file
            printf("[EOF MENTION] Thank you for using!\n");
            exit(0);
        }

        /* check if userInput only has a '\n', which means user didn't
           input anything and just press the ENTER key */ 
        if (strcmp(userInput, "\n") == 0)
        {
            printf("[ERROR] Invalid command arguement!\n");
            continue;
        }

        // fgets will keeps '\n' when user input
        strtok(userInput, "\n");


        // printf("%s\n", &userInput);

        // separate userInput by space
        char * ptr = strtok(userInput, " ");

        for (int i = 0; ptr != NULL; i++)
        {
            // printf("%s\n", ptr);
            argv[i] = ptr;
            ptr = strtok(NULL, " ");
            keep = i;
        }

        
        argv[keep + 1] = NULL;

        // used to check each arugement should be in argv
        // printf("size of argv: %ld\n", strlen(argv));
        // for (int i = 0; argv[i] != NULL; i++)
        // {
        //     printf("%d: %s\n", i, argv[i]);
        // }

        //check if user want to exit
        if (strcmp(argv[0], "exit") == 0)
        {
            exit(0);
        }

        pid_t process_id; // initialize process id

        int status;
        int ret = fork(); // creating a child process

        // if (ret != 0)
        // {
        //     wait(NULL);
        // }

        // 
        switch(ret)
        {
            case 0: // child process
                /* once execvp successfully,  overlay a process image 
                    that has been created by a call to the fork function*/
                execvp(argv[0], argv);
            case -1: // fork faild
                printf("Invalid input failed. Exit with error %d\n", errno);
                exit(2);
            default: // parent process

                // the parent process must wait upon the terminate of child process
                process_id = wait(&status);

                // query status to see if a child process ended normally
                if (WIFEXITED(status))
                {
                    // print child process id, and exit status of child
                    printf("Child %d, exited with %d\n", process_id, WEXITSTATUS(status));
                }

        }
    }
}



