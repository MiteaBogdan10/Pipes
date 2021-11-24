#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

#define READ 0
#define WRITE 1
#define SIZE 10000 //defining the size of the array
#define PROCESS 10 //defining the number of childs

using namespace std;

void primeNumber(int arr[SIZE], int proc,int writePipe, int readPipe) //function for checking if the numbers are prime with a flag(isPrime)
{
    close(readPipe); //closing the read end pipe because we do not use it
    for(int k = proc; k<SIZE; k+=PROCESS) //I used shell sort to split the numbers
    {
        bool isPrime = true;
        for(int i  = 2; i*i<= arr[k];i++)
        {
            if(arr[k]%i==0){
                isPrime = false;
                break;
            }
        }
        if(isPrime)
        {
            write(writePipe,&arr[k],sizeof(arr[k])); //write if the flag is true.
        }
    }
    close(writePipe);
    exit(1);
}

int main()
{
     int i = 0;
     int pArr[SIZE];
    int myPipe[2];
    pipe(myPipe); //creating the pipe

    int array[SIZE];

    do //initializing the array with consecutive numbers
    {
        array[i] = i+1;
        i++;
    } while (SIZE > i);

    for(int i = 0; i < SIZE; i++) //loop for printing all the numbers from the array
    {
        cout<<array[i]<<" ";
    }

    cout<<endl;
    cout<<endl;


    pid_t child[PROCESS]; //initializing the number of children
    for(size_t j = 0 ; j < PROCESS ; j++)
    {
        child[j] = fork(); //creating child process
        if(child[j]<0)
        {
            cout<<"Error creating the fork";
        }
        else if(child[j]==0) //accessing the child
        {
            primeNumber(array, j, myPipe[WRITE], myPipe[READ]); //calling the function in every child
        }
    }


    int returnStatus;
    for(int i = 0 ; i < PROCESS ; i++) // WAITING FOR ALL PROCESSES TO STOP
        waitpid(child[i], &returnStatus, 0);


    int stop=-1;
    write(myPipe[1],&stop,sizeof(int)); //sending -1 at the end of the written prime number for knowing when to stop printing

    close(myPipe[WRITE]);

    for(int j = 0; j< PROCESS;j++)
        read(myPipe[READ],&pArr,sizeof(pArr)); //reading the values that have been written from the processes

    close(myPipe[0]);
   int p=0; //index for array
   do
    {
        cout<<pArr[p]<<" ";
        p++;
    } while (pArr[p]!=-1); //printing until we reach -1 (the one we added earlier)
    cout<<endl;
    cout<<"We have "<<p<<" prime numbers";

    cout<<endl;

    wait(NULL);
    return 0;
}
