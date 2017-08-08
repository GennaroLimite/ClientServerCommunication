#include <stdio.h> //include standard I/O lirary
#include <stdlib.h>
#include <unistd.h> //include unix standard library
#include <sys/types.h> //predefined types
#include <sys/socket.h> //socket library
#include <arpa/inet.h> //IP address conversion utilities
#include <string.h>
#include <errno.h>

typedef struct negozio
{
        int  ID;
        char Nome[20];
	char Descrizione[250];
	int  PosizioneX;
	int PosizioneY;
	int Piano;
}NEGOZIO;

NEGOZIO ArrayNegozio[100];

