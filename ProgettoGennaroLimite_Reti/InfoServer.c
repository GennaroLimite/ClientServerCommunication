#include "header.h"

void CaricaServizi();
//this is the list
char List[] ="1)  Nike\n2)  Adidas\n3)  NewBalance\n4)  H&M\n5)  Bershka\n6)  Paul&Bear\n7)  FootLocker\n8)  Alcott\n9)  OVS\n10) Tezenis\n11) Zara\n12) Reebok\n13) Puma\n14) Pandora\n15) Oysho\n16) Accessorize\n17) Macron\n18) Bialetti\n19) Camomilla\n20) Apple\n21) De Fonseca\n22) NeroGiardini\n23) Piazza Italia\n\n";

int main(int argc, char **argv)
{
	int list_fd,conn_fd,route_fd,i,choice1,choice2,n,posX[1],posY,floor;
	char buff[1024], buffI[1024],buffer[2048];
	char recevline[2048];
	char percorso[2048];
	struct sockaddr_in servaddr,routeaddr;
        pid_t pid;
	//socket
	if((list_fd = socket(AF_INET,SOCK_STREAM,0))<0){
	perror("socket");
	exit(1);
	}
	//
	servaddr.sin_family =AF_INET;
	servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	servaddr.sin_port=htons(1024);
	//bind
	if (bind(list_fd,(struct sockaddr *)&servaddr,sizeof(servaddr))<0){
	perror("bind");
	exit(1);
	}
	//listen
	if(listen(list_fd,1024)<0){
	perror("listen");
	exit(1);
	}
    while(1)
    {
		//accept
		if((conn_fd = accept(list_fd, (struct sockaddr *) NULL, NULL)) < 0) {
		perror("accept");
		exit(1);
		}
		//fork
		if((pid=fork())<0){
		perror("Fork error");
		exit(-1);
		}
		//son
   		if(pid==0)
		{
	 		close (list_fd);
      		while(1)
		{
			//write the list to the client
			snprintf(buff,sizeof(buff),"%s\r\n",List);
            		write(conn_fd,buff,sizeof(buff));
			//read the service chosen from client
			read(conn_fd, &choice1, sizeof(int));
			CaricaServizi();
			//write the description of the chosen service
			snprintf(buff,sizeof(buff),"\nDescrizione:\n%s\r\n",ArrayNegozio[choice1].Descrizione);
			write(conn_fd,buff,sizeof(buff));
			//write the position of the chosen service
			snprintf(buff,sizeof(buff),"X :%d\r\nY:%d\r\nPIANO:%d\r\n\n",ArrayNegozio[choice1].PosizioneX,ArrayNegozio[choice1].PosizioneY,ArrayNegozio[choice1].Piano);
			write(conn_fd,buff,sizeof(buff));
			//read the choice
			read(conn_fd, &choice2, sizeof(int));
			switch(choice2)
			{
			case 1:
			 //read the client position
			 read(conn_fd, &posX, sizeof(int));

			 read(conn_fd, &posY, sizeof(int));

			 read(conn_fd, &floor, sizeof(int));

			 //creazione della socket per interagire con il routeserver
			 if((route_fd = socket(AF_INET,SOCK_STREAM,0))<0){
			 perror("socket");
			 exit(1);
			 }
			 //
			 routeaddr.sin_family = AF_INET;
   			 routeaddr.sin_port = htons(1025);
			 
			 if (inet_pton(AF_INET,"127.0.0.1", &routeaddr.sin_addr) <= 0)
   			 {
     			 fprintf(stderr,"inet_pton error for %s\n", argv[1]); 
			 exit (1);
			 }
			 //connect
			 if (connect(route_fd, (struct sockaddr *) &routeaddr, sizeof(routeaddr)) < 0) {
			 fprintf(stderr,"connect error\n");
			 exit(1);
			 }
			 //scrivo la posizione del client e del servizio scelto al rootserver
	        	
			write(route_fd, &posX, sizeof(int));

			write(route_fd, &posY, sizeof(int)); 

			write(route_fd, &floor, sizeof(int));

			write(route_fd, &ArrayNegozio[choice1].PosizioneX, sizeof(int));

			write(route_fd, &ArrayNegozio[choice1].PosizioneY, sizeof(int));

			write(route_fd, &ArrayNegozio[choice1].Piano, sizeof(int));
			//leggo la risposta del routeserver e la copio all'interno della variabile percorso
			read(route_fd, recevline, 2048);		
			recevline[strlen(recevline)]='\0';
			strcpy(percorso,recevline);
			//mando il percorso al client
			snprintf(buffer,sizeof(buffer),"%s",percorso);
			if (write(conn_fd,buffer,strlen(buffer)) != strlen(buffer))
			{
				perror("write");
				exit(1);			
			}
			 exit(1);
			 break;
			case 2:	
			 exit(1);
			 break;
			}
		}		
		close(conn_fd);
    	}
		close(conn_fd);
	}
	exit(0);
}

void CaricaServizi()
{
FILE *fd;
char buf[250];
char *res;
char i,xi,yi,pi;

fd=fopen("CaricaServizi.txt", "r");
if( fd==NULL )  {
    perror("Errore in apertura del file");
    exit(1);
  		}

for(i=0;i<24;i++)
        {
	xi=atoi(fgets(buf, 250, fd));
	yi=atoi(fgets(buf, 250, fd));
	pi=atoi(fgets(buf, 250, fd));
	res=(fgets(buf, 250, fd));
	strcpy(ArrayNegozio[i].Descrizione, res); 
    	res=(fgets(buf, 250, fd));
	strcpy(ArrayNegozio[i].Nome, res);  
	ArrayNegozio[i].ID=(i+1);
	ArrayNegozio[i].PosizioneX=xi;
 	ArrayNegozio[i].PosizioneY=yi;
	ArrayNegozio[i].Piano=pi;       
	}
fclose(fd);
}
