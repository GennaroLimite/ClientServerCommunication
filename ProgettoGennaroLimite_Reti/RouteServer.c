#include "header.h"

char* Percorso(int posinix, int posiniy,int posinip,int posfinix,int posfiniy,int posfinip);

int main(int argc, char **argv)
{
	int Rlist_fd,Rconn_fd,i,choice1,choice2,n,ClientX,ClientY,ClientFloor,ServiceX,ServiceY,ServiceFloor;
	char Rbuff[2048];
	char Rrecevline[1024];
	char *PERCORSO;
	struct sockaddr_in servaddr, routeaddr;
        pid_t pid;
	//socket
	if((Rlist_fd = socket(AF_INET,SOCK_STREAM,0))<0){
	perror("socket");
	exit(1);
	}
	//
	routeaddr.sin_family =AF_INET;
	routeaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	routeaddr.sin_port=htons(1025);
	//bind
	if (bind(Rlist_fd,(struct sockaddr *)&routeaddr,sizeof(routeaddr))<0){
	perror("bind");
	exit(1);
	}
	//listen
	if(listen(Rlist_fd,1024)<0){
	perror("listen");
	exit(1);
	}
	while(1)
	{
	//accept
		if((Rconn_fd = accept(Rlist_fd, (struct sockaddr *) NULL, NULL)) < 0) {
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
	 		close (Rlist_fd);
      			while(1)
			{
			//leggo la posizione del client e del servizio scelto dal client

			read(Rconn_fd, &ClientX, sizeof(int));

			read(Rconn_fd, &ClientY, sizeof(int));

			read(Rconn_fd, &ClientFloor, sizeof(int));

			read(Rconn_fd, &ServiceX, sizeof(int));

			read(Rconn_fd, &ServiceY, sizeof(int));

			read(Rconn_fd, &ServiceFloor, sizeof(int));				

			//chiamata alla function per calcolare il percorso che mi ritorna char percorso[]
			PERCORSO=Percorso(ClientX,ClientY,ClientFloor,ServiceX,ServiceY,ServiceFloor);
			//mando il percorso all'infoserver
			snprintf(Rbuff,sizeof(Rbuff),"%s",PERCORSO);
			write(Rconn_fd,Rbuff,strlen(Rbuff));
			exit(1);
			}
		close(Rconn_fd);
		}
	close(Rconn_fd);	
	}
exit(0);
}



char* Percorso(int posinix, int posiniy,int posinip,int posfinix,int posfiniy,int posfinip)
{
FILE *fd,*fd2;
char *percorso=(char*) malloc (4000* sizeof(char));
char buf[200], buf2[200];
char buffer[16];
char *res;
char iterator,i,xi,yi,pi;

/* apre il file */
fd=fopen("Coordinate.txt", "r");
if( fd==NULL )  
{
perror("Errore in apertura del file");
exit(1);
}
//cerco la posizione del client all'interno del file
for(i=0;i<25;i++)
        {
	xi=atoi(fgets(buf, 200, fd));
	yi=atoi(fgets(buf, 200, fd));
	pi=atoi(fgets(buf, 200, fd));
    	res=(fgets(buf, 200, fd));
	ArrayNegozio[i].PosizioneX=xi;
 	ArrayNegozio[i].PosizioneY=yi;
	ArrayNegozio[i].Piano=pi;
	strcpy(ArrayNegozio[i].Nome, res);
	iterator=i;
        if( xi==posinix && yi==posiniy && pi==posinip)
                break;
        }
if(posinip<posfinip)
{
//inserisco in percorso i nomi che separano il client dal servizio scelto
for(i=iterator;i<25;i++)
	{
    	xi=atoi(fgets(buf, 200, fd));
    	yi=atoi(fgets(buf, 200, fd));
    	pi=atoi(fgets(buf, 200, fd));
    	res=(fgets(buf, 200, fd));
    	res[strlen(res)-1]='\0';
    	ArrayNegozio[i].PosizioneX=xi;
    	ArrayNegozio[i].PosizioneY=yi;
    	ArrayNegozio[i].Piano=pi;
    	strcpy(ArrayNegozio[i].Nome, res);
	strcat(percorso,"-----> ");
	if(ArrayNegozio[i].Piano != ArrayNegozio[i-1].Piano)
		strcat(percorso,"Vai al piano superiore\n-----> ");
	strcat(percorso,ArrayNegozio[i].Nome);
	strcat(percorso," Posizione X: ");
	snprintf(buffer,sizeof(buffer),"%d",ArrayNegozio[i].PosizioneX);
	strcat(percorso,buffer);
	strcat(percorso," Posizione Y: ");
	snprintf(buffer,sizeof(buffer),"%d",ArrayNegozio[i].PosizioneY);
	strcat(percorso,buffer);
	strcat(percorso," Piano: ");
	snprintf(buffer,sizeof(buffer),"%d",ArrayNegozio[i].Piano);
	strcat(percorso,buffer);
	strcat(percorso,"\n");
	iterator=i;
	if( xi==posfinix && yi==posfiniy && pi==posfinip)
		break;	
	}
fclose(fd);
}else if(posinip==posfinip) 
	{
		/* apre il file */
		fd=fopen("Coordinate.txt", "r");
		if( fd==NULL )  
		{
    		perror("Errore in apertura del file");
    		exit(1);
  		}
		for(i=0;i<25;i++)
        		{
			xi=atoi(fgets(buf, 200, fd));
			yi=atoi(fgets(buf, 200, fd));
			pi=atoi(fgets(buf, 200, fd));
    			res=(fgets(buf, 200, fd));
			ArrayNegozio[i].PosizioneX=xi;
 			ArrayNegozio[i].PosizioneY=yi;
			ArrayNegozio[i].Piano=pi;
			strcpy(ArrayNegozio[i].Nome, res);
        		if( xi==posfinix && yi==posfiniy && pi==posfinip)
                		break;
			}
fclose(fd);
	 if(iterator>i)
		{
			/* apre il file */
			fd2=fopen("Coordinate1.txt", "r");
			if( fd==NULL )  
			{
    			perror("Errore in apertura del file");
    			exit(1);
	  		}
		//cerco la posizione del client all'interno del file
		for(i=0;i<25;i++)
		        {
			xi=atoi(fgets(buf2, 200, fd2));
			yi=atoi(fgets(buf2, 200, fd2));
			pi=atoi(fgets(buf2, 200, fd2));
    			res=(fgets(buf2, 200, fd2));
			ArrayNegozio[i].PosizioneX=xi;
 			ArrayNegozio[i].PosizioneY=yi;
			ArrayNegozio[i].Piano=pi;
			strcpy(ArrayNegozio[i].Nome, res);
			iterator=i;
        		if( xi==posinix && yi==posiniy && pi==posinip)
                		break;
        		}
		for(i=iterator;i<25;i++)
	 		{
			xi=atoi(fgets(buf2, 200, fd2));
    			yi=atoi(fgets(buf2, 200, fd2));
    			pi=atoi(fgets(buf2, 200, fd2));
    			res=(fgets(buf2, 200, fd2));
    			res[strlen(res)-1]='\0';
    			ArrayNegozio[i].PosizioneX=xi;
    			ArrayNegozio[i].PosizioneY=yi;
    			ArrayNegozio[i].Piano=pi;
    			strcpy(ArrayNegozio[i].Nome, res);
			strcat(percorso,"-----> ");
			strcat(percorso,ArrayNegozio[i].Nome);
			strcat(percorso," Posizione X: ");
			snprintf(buffer,sizeof(buffer),"%d",ArrayNegozio[i].PosizioneX);
			strcat(percorso,buffer);
			strcat(percorso," Posizione Y: ");
			snprintf(buffer,sizeof(buffer),"%d",ArrayNegozio[i].PosizioneY);
			strcat(percorso,buffer);
			strcat(percorso," Piano: ");
			snprintf(buffer,sizeof(buffer),"%d",ArrayNegozio[i].Piano);
			strcat(percorso,buffer);
			strcat(percorso,"\n");
			iterator=i;
			if( xi==posfinix && yi==posfiniy && pi==posfinip)
				break;						
			}
fclose(fd2);		
		}else {
			/* apre il file */
			fd=fopen("Coordinate.txt", "r");
			if( fd==NULL )  
			{
    			perror("Errore in apertura del file");
    			exit(1);
  			}
			//cerco la posizione del client all'interno del file
			for(i=0;i<25;i++)
        			{
				xi=atoi(fgets(buf, 200, fd));
				yi=atoi(fgets(buf, 200, fd));
				pi=atoi(fgets(buf, 200, fd));
    				res=(fgets(buf, 200, fd));
				ArrayNegozio[i].PosizioneX=xi;
 				ArrayNegozio[i].PosizioneY=yi;
				ArrayNegozio[i].Piano=pi;
				strcpy(ArrayNegozio[i].Nome, res);
				iterator=i;
        			if( xi==posinix && yi==posiniy && pi==posinip)
                			break;
        			}
			for(i=iterator;i<25;i++)
				{
    				xi=atoi(fgets(buf, 200, fd));
    				yi=atoi(fgets(buf, 200, fd));
    				pi=atoi(fgets(buf, 200, fd));
    				res=(fgets(buf, 200, fd));
    				res[strlen(res)-1]='\0';
    				ArrayNegozio[i].PosizioneX=xi;
    				ArrayNegozio[i].PosizioneY=yi;
    				ArrayNegozio[i].Piano=pi;
    				strcpy(ArrayNegozio[i].Nome, res);
				strcat(percorso,"-----> ");
				strcat(percorso,ArrayNegozio[i].Nome);
				strcat(percorso," Posizione X: ");
				snprintf(buffer,sizeof(buffer),"%d",ArrayNegozio[i].PosizioneX);
				strcat(percorso,buffer);
				strcat(percorso," Posizione Y: ");
				snprintf(buffer,sizeof(buffer),"%d",ArrayNegozio[i].PosizioneY);
				strcat(percorso,buffer);
				strcat(percorso," Piano: ");
				snprintf(buffer,sizeof(buffer),"%d",ArrayNegozio[i].Piano);
				strcat(percorso,buffer);
				strcat(percorso,"\n");
				iterator=i;
				if( xi==posfinix && yi==posfiniy && pi==posfinip)
					break;
				}
			fclose(fd);			
			}
	}else{
		/* apre il file */
		fd2=fopen("Coordinate1.txt", "r");
		if( fd==NULL )  
		{
    		perror("Errore in apertura del file");
    		exit(1);
  		}
		//cerco la posizione del client all'interno del file
		for(i=0;i<25;i++)
        		{
			xi=atoi(fgets(buf2, 200, fd2));
			yi=atoi(fgets(buf2, 200, fd2));
			pi=atoi(fgets(buf2, 200, fd2));
    			res=(fgets(buf2, 200, fd2));
			ArrayNegozio[i].PosizioneX=xi;
 			ArrayNegozio[i].PosizioneY=yi;
			ArrayNegozio[i].Piano=pi;
			strcpy(ArrayNegozio[i].Nome, res);
			iterator=i;
        		if( xi==posinix && yi==posiniy && pi==posinip)
                		break;
        		}

		//inserisco in percorso i nomi che separano il client dal servizio scelto
		for(i=iterator;i<25;i++)
			{
    			xi=atoi(fgets(buf2, 200, fd2));
    			yi=atoi(fgets(buf2, 200, fd2));
    			pi=atoi(fgets(buf2, 200, fd2));
    			res=(fgets(buf2, 200, fd2));
    			res[strlen(res)-1]='\0';
    			ArrayNegozio[i].PosizioneX=xi;
    			ArrayNegozio[i].PosizioneY=yi;
    			ArrayNegozio[i].Piano=pi;
    			strcpy(ArrayNegozio[i].Nome, res);
			strcat(percorso,"-----> ");
			if(ArrayNegozio[i].Piano != ArrayNegozio[i-1].Piano && i!=0)
				strcat(percorso,"Vai al piano inferiore\n-----> ");
			strcat(percorso,ArrayNegozio[i].Nome);
			strcat(percorso," Posizione X: ");
			snprintf(buffer,sizeof(buffer),"%d",ArrayNegozio[i].PosizioneX);
			strcat(percorso,buffer);
			strcat(percorso," Posizione Y: ");
			snprintf(buffer,sizeof(buffer),"%d",ArrayNegozio[i].PosizioneY);
			strcat(percorso,buffer);
			strcat(percorso," Piano: ");
			snprintf(buffer,sizeof(buffer),"%d",ArrayNegozio[i].Piano);
			strcat(percorso,buffer);
			strcat(percorso,"\n");
			iterator=i;
			if( xi==posfinix && yi==posfiniy && pi==posfinip)
				break;
			}	
fclose(fd2);
	}
return (char*)percorso;
}
