/* TCPecho.c - main, TCPecho */
#include <unistd.h> 				/*Defines constants type and function declarations that overrides POSIX OS API*/
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>				 /*Defines different system error numbers which has designated meaning*/
#include <pthread.h>				 /*Defines various thread functions */
#include <sys/types.h>				 /*Defines the behaviour of each of the functions listed under this */
#include <sys/socket.h>				 /*Defines functions used to perform socket related operations*/
#include <netinet/in.h>				 /*Defines constant and structures needed for Internet domain addresses*/
#include <arpa/inet.h>
#include <netdb.h> 
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include<semaphore.h>
#define LISTENQ 10
#define MAXLINE 1000

sem_t mutex;

FILE *fp;
void add_Airliner_database(sqlite3*,int);
void display_Airliner_database(sqlite3*,int);					 /*fp is a file pointer of type FILE  */
void delete_Airliner_database(sqlite3*,int);
void create_Airliner_database(sqlite3*,int);
void airliner_database(int);

void add_database(sqlite3*,int);
void update_database(sqlite3*,int);
void delete_database(sqlite3*,int);
void display_database(sqlite3*,int);
void create_database(sqlite3*,int);

void agent_add_database(sqlite3*,int);
void agent_update_database(sqlite3*,int);
void agent_delete_database(sqlite3*,int);
void agent_display_database(sqlite3*,int);
void agent_create_database(sqlite3*,int);

int asprintf(char**,char*,...);
static void *doit(void *);
static void err_doit(int, int, const char *, va_list);
void err_quit(const char *fmt, ...);
void err_sys(const char *fmt, ...);

/* each thread executes this function */
int tcp_listen(const char *host, const char *serv, socklen_t *addrlenp);
void str_echo(int fd);

void handle_customer(void * fd);
void handle_sysadmin(int fd);
void handle_airliner(int fd);
void handle_bookings(void * fd);

void customer_database(int fd);

int main(int argc, char **argv)		        /*Argument Count and Argument Value Parameters are passed to the main function */
{
	int listenfd, *iptr;
	pthread_t tid;
	socklen_t addrlen, len;
	struct sockaddr *cliaddr;

	if (argc == 2)
		listenfd = tcp_listen(NULL, argv[1], &addrlen);
	else if (argc == 3)
		listenfd = tcp_listen(argv[1], argv[2], &addrlen);
	else
		err_quit("usage: tcpserv01 [ <host> ] <service or port>");

	cliaddr = malloc(addrlen);				 //used for dynamic memory allocation 

	for ( ; ; ) {						 //infinte loop	
		len = addrlen; 
		iptr = malloc(sizeof(int));			 //used for dynamic memory allocation 
		*iptr = accept(listenfd, cliaddr, &len);	 // accept the incoming connections and place in iptr pointer
		 sem_init(&mutex,0,1);
                 pthread_create(&tid, NULL, &doit, iptr);	 // calling create thread functions
	}
}

static void * doit(void *arg)					 // The request will be controlled by the threads created
{
	int connfd;
	connfd = *((int *) arg);
	free(arg);
	pthread_detach(pthread_self());
        sem_destroy(&mutex);
	str_echo(connfd);					 /* same function as before */
	close(connfd);						 /* done with connected socket */
	return (NULL);
}


int tcp_listen(const char *host, const char *serv, socklen_t *addrlenp)
{
	int listenfd, n,join,list,length;
	const int on = 1;
        struct sockaddr_in6 server;

	/*struct addrinfo hints, *res, *ressave;
	bzero(&hints, sizeof (struct addrinfo)) ;
	hints.ai_flags = AI_PASSIVE;
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
       */
bzero((char *)&server,sizeof(server));
server.sin6_flowinfo= 0;
server.sin6_family= AF_INET6;                             
server.sin6_port=htons(atoi(serv));			
server.sin6_addr=in6addr_any;			



             listenfd=socket(AF_INET6, SOCK_STREAM, 0);	
if(listenfd==-1)
{
printf("Socket is not created\n");		 	// Error when socket not created
exit(-1);
}

length=sizeof(server);
join=bind(listenfd, (struct sockaddr *)&server, length);
if (join==-1)
	{
	printf("Binding failed\n");			// error when bind does not happend
	exit(-1);
}


list=listen(listenfd,LISTENQ);					// passing arguments sd, and queue to the listen function
if (list==-1)
{
printf("Listen has not happened\n");			// Error when listen function fails
exit(-1);
}
/*
	if ( (n = getaddrinfo (host, serv, &hints, &res)) != 0)
		err_quit("tcp_listen error for %s, %s: %s",host, serv, gai_strerror(n)) ;

	ressave = res;
	do {
		listenfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
		if (listenfd < 0)
			continue;					 /* error, try next one 
		setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof (on) ) ;

		if (bind(listenfd, res->ai_addr, res->ai_addrlen) == 0)
			break;						 /* success 
		close (listenfd);					 /* bind error, close and try next one 
	}while ( (res = res->ai_next) != NULL);

	if (res == NULL)						 /* errno from final socket () or bind () 
		err_sys ("tcp_listen error for %s, %s", host, serv);
	listen (listenfd, LISTENQ);
	if (addrlenp)
		*addrlenp = res->ai_addrlen;				 /* return size of protocol address 
	freeaddrinfo (ressave*/
	return (listenfd);

}

void str_echo(int fd)
{
	char buf[1000];
	char buf1[1000];
        char welcome_buf[1000];
	char bye_buf[1000];
	char exit[1000];
	int cc;
	double value;

       Main:
         fp=fopen("welcome.txt","r");
         fread(welcome_buf,1000,1,fp); 
         write(fd,welcome_buf,1000);
         fclose(fp);
	
	while(cc= read(fd, buf, 1000))
	{
        if(atoi(buf)==1)
        {

	    handle_customer((void *)fd); 
		
		goto Main;		       

         }

        else if(atoi(buf)==2)
        {
		handle_sysadmin(fd);
               
                 goto Main;
        }

        else if(atoi(buf)==3)
        {
          
	       handle_airliner(fd);
		goto Main;
 	}  
        else if (strcmp(buf,"b\n")==0)
        {
            
	 fp=fopen("Thank_you.txt","r");
         fread(bye_buf,1000,1,fp); 
         write(fd,bye_buf,1000);
         fclose(fp);
	}

	else if (strcmp(buf,"f\n")==0)
	goto Main;
       
        else if(strcmp(buf,"exit\n")==0)
         {
             bzero(exit,1000);
             write(fd,"10",5);
}
	}
}




/*-------------------------------------------------------------      System Admin Part -----------------------------------------------------*/

void handle_sysadmin(int fd)

{

	char admin[1000];
	char admin_newuser[1000];
	char readbuf[1000];
	int cc;
        char path[1000];
        char path1[1000];
               server:
           
   		printf("this is the adminstrator part\n");
		fp=fopen("Administrator_login_existing.txt","r");
		fread(admin,1000,1,fp);
		write(fd,admin,1000);
		fclose(fp);
   
        //printf("brfore read\n");
       cc= read(fd, readbuf, 1000);

         if(cc<0)
         printf("Read failed\n");


           if(strcmp(readbuf,"b\n")==0)
                 {
                      
                       return;
                  }


          if(strcmp(readbuf,"root root@123\n")==0)
		{
		
	            printf("authentication is successfull\n");
                    write(fd,"authentication is successfull\n",30);

char admin_options_buf[1000];
                   database:
                    

fp=fopen("admin_options.txt","r");
fread(admin_options_buf,1000,1,fp);
write(fd,admin_options_buf,strlen(admin_options_buf));
fclose(fp);




}

       
 bzero(readbuf,1000);
 cc=read(fd,readbuf,1000);
if(strcmp(readbuf,"b\n")==0)
{
printf("go back to root login page\n");
goto server;
}

if(atoi(readbuf)==1)
{
char admin_options_buf[1000];

subop:

fp=fopen("admin_flights.txt","r");
fread(admin_options_buf,1000,1,fp);
write(fd,admin_options_buf,strlen(admin_options_buf));
fclose(fp);



             bzero(readbuf,1000);    
          while(  cc=read(fd,readbuf,1000))
{      
   sqlite3 *db;
   char *zErrMsg = 0;
   int  rc;
  
   char *sql;
   const char* data = "Callback function called";
   /* Open database */
   rc = sqlite3_open("mydb_flight.db", &db);
   if( rc ){
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
   }else{
      fprintf(stdout, "Opened database successfully\n");
   }
     

if(strcmp(readbuf,"b\n")==0)
{
printf("go back to page with the data base selection\n");
goto database;
}

  switch(atoi(readbuf)){
    case 0:
               printf("create a database table\n");
                 create_database(db,fd);
                  goto subop;
                 break;

    case 1:
                printf("add to the data base\n");
                  add_database(db,fd);
                   goto subop;
                  break;

    case 2:
                printf("update the data base\n");
                  update_database(db,fd);
                    goto subop;
                break;
    case 3:
                printf("delete the data base\n");
                 delete_database(db,fd);
                    goto subop;
                 break;

    case 4:   
               printf("display the data base\n");
                 display_database(db,fd);
               cc=read(fd,readbuf,1000);
                if(strcmp(readbuf,"b\n")==0)
                {

                       goto subop;
                  }  
                break;
}
}
   

}

else if(atoi(readbuf)==2)
{

char admin_airliner_options_buf[1000];
subop_airline:
fp=fopen("admin_airliner_op.txt","r");
fread(admin_airliner_options_buf,1000,1,fp);
write(fd,admin_airliner_options_buf,strlen(admin_airliner_options_buf));
fclose(fp);




          bzero(readbuf,1000);  
          while(  cc=read(fd,readbuf,1000))
{      
   sqlite3 *db;
   char *zErrMsg = 0;
   int  rc;
   char *sql;
   const char* data = "Callback function called";
   /* Open database */
   rc = sqlite3_open("mydb_Airliner.db", &db);
   if( rc ){
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
   }else{
      fprintf(stdout, "Opened database successfully\n");
   }

if(strcmp(readbuf,"b\n")==0)
{
printf("go back to page with the data base selection\n");
goto database;
}

  switch(atoi(readbuf)){
    case 0:
               printf("create a database table\n");
                 create_Airliner_database(db,fd);
goto subop_airline;
                 break;

    case 1:
                printf("add to the data base\n");
                  add_Airliner_database(db,fd);
goto subop_airline;
                  break;

    case 2:
                printf("Delete the data base\n");
                  delete_Airliner_database(db,fd);
goto subop_airline;
                break;
    case 3:
                printf("Display the data base\n");
                 display_Airliner_database(db,fd);
cc=read(fd,readbuf,1000);
                if(strcmp(readbuf,"b\n")==0)
                {

                       goto subop_airline;
                  }  
                 break;

}
}
 
}
else if(atoi(readbuf)==3)
{
      printf("Back up of the data base\n");
      write(fd,"Welcome to Data Back up system\n\n",30);
      write(fd,"\n\nEnter the path where the data needs to be backed up <<PATH>>",80);
      bzero(path,1000);
      bzero(readbuf,1000);
      bzero(path1,1000);

  
      cc=read(fd,readbuf,1000);

if(strcmp(readbuf,"b\n")==0)
{
printf("go back to page with the data base selection\n");
goto database;


}

      strtok(readbuf,"\n");
      printf("path chosen for backup:%s\n",readbuf);
      strcat(path1,"mkdir ");
      strcat(path1,readbuf);
      strcat(path1,"/BACKUP");
      printf("directory path of backup %s\n",path1);
      system(path1);
       
      strcat(path,"cp -r *.db");
     printf("directory created\n"); 
      strcat(path," ");
      strcat(path,readbuf);

     strtok(path,"\n");
      strcat(path,"/BACKUP/");
      printf("Command:%s\n",path);
      system(path);
      write(fd,"Data Backup Successfull",30);
bzero(readbuf,1000);
cc=read(fd,readbuf,1000);
                if(strcmp(readbuf,"b\n")==0)
                {

                       goto database;
                }





}

}

/*-----------------------------------------                        Airliner part      --------------------------------------------------*/ 
void handle_airliner(int fd)

{

	char airline[1000];
	char airline_newuser[1000];
	char readbuf[1000];
	int cc;
              // l:
		printf("this is the flight assistant part\n");
		fp=fopen("CommercialAirline_login_existing .txt","r");
		fread(airline,1000,1,fp);
		write(fd,airline,1000);
		fclose(fp);
                     bzero(readbuf,1000);
		cc= read(fd, readbuf, 1000);
			if(cc<0)
			printf("Read not successfull");  
		
                       printf("%s",readbuf);
 if(strcmp(readbuf,"b\n")==0)
                 {
                      
                       return;
                  }



if((strcmp(readbuf,"nagthej_manangi sjsu@1234\n")==0)) 
{
printf("Authenticated!\n");
airliner_database(fd);

}

		
		

}






