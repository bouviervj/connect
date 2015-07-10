#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <lib/socket/socket.h>

int main(int argc, char *argv[])
{

    if(argc != 2)
    {
        printf("\n Usage: %s <ip of server> \n",argv[0]);
        return 1;
    } 

    net::socketconnect aSck(argv[1], 5000);

    pthread_exit(0);    	    

    return 0;
}
