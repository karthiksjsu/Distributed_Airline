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
#define LISTENQ 10
#define MAXLINE 1000

FILE *fp;					 /*fp is a file pointer of type FILE  */
sqlite3* customer_create_database(int);
static int count=0;
sqlite3 *db;
char *zErrMsg = 0;
int  rc;

void customer_update_database(sqlite3*,int);
void customer_delete_database(sqlite3*,int);
void customer_add_database(sqlite3*,int);
void handle_bookings(sqlite3*,void*);
  
void handle_customer(void * sock)

{
	int fd = (int) sock;
	char client_newuser1[1000];
	char client[1000];
	char readbuf[1000];
	char login1[1000];
	const char* names[] = {"pramod prakash","karthik siddalingappa","nagthej manangi"}; 
	int cc;
        char *zErrMsg = 0;
  	int  rc;
	char *sql;
	const char* data = "Callback function called";

   /* Open database*/ 
   rc = sqlite3_open("mydb_Customer.db", &db);
   if( rc )
	{
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
  	 }
   else{
      fprintf(stdout, "Opened database successfully\n");
       }
                 printf("value of the db before %p\n",db);

	customer:
		printf("this is the client function part\n"); 
		fp=fopen("Customer_login_existing.txt","r");
		bzero(client,1000);
		fread(client,1000,1,fp);
		write(fd,client,strlen(client));
		fclose(fp);

		while(1)

		{ 
		    
		    cc= read(fd, readbuf, 1000);
		    readbuf[cc]='\0';
		  
		
		if (atoi(readbuf)==1)
		{	
			printf("this is the new customer function part\n");
			fp=fopen("Customer_login_newuser.txt","r");
			bzero(client_newuser1,1000);
			fread(client_newuser1,1000,1,fp);
			write(fd,client_newuser1,strlen(client_newuser1));
			fclose(fp);
   			bzero(readbuf,1000);

                        customer_add_database(db, fd);
			printf("Login successful...\n");
				
			handle_bookings(db,(void *)fd);
			goto customer;
			
		}
		 
	  		
		if((strcmp(readbuf,"pramod_prakash sjsu@1234\n")==0)||(strcmp(readbuf,"karthik_siddalingappa sjsu@1234\n")==0)||(strcmp(readbuf,"nagthej_manangi sjsu@1234\n")==0))
		{
			printf("Login successful for %s\n",readbuf);
			
			customer1:
			bzero(readbuf,1000);	
			handle_bookings(db,(void *)fd);
			goto customer;
			
		}
		
		if(strcmp(readbuf,"b\n")==0)
		{
			bzero(readbuf,1000);
			return;
		}
}

}


handle_flight_bookings(void * sock)
{
	int fd = (int) sock;
	int cc;
	char from_to_flights[1500];
	char readbuf[1000];
	char *zErrMsg = 0;
	int  rc;


                      /* Open database */

                        rc = sqlite3_open("mydb_flight.db", &db);
                     if( rc ){
                             fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
                        }else{
                              fprintf(stdout, "Opened database successfully\n");
                               }
                           
                        customer_display_database( db, fd);
		
				
			
}

void book_hotel(void* sock)
{	

	int fd = (int) sock;
	int cc;
	char bookings[1000];
	char readbuf[1000];
while(cc= read(fd, readbuf, 1000))
	{

if(strcmp(readbuf,"sanjose\n")==0)
		{	
			printf("Hotel in the City : %s \n",readbuf);
			fp=fopen("Hotel_Display.txt","r");			
			bzero(bookings,1000);
			fread(bookings,1000,1,fp);
			write(fd,bookings,1000);
			fclose(fp);
			bzero(readbuf,1000);
		}
if(strcmp(readbuf,"b\n")==0)
		return;
		

	}
}

void book_a_cab(void* sock)
{	

	int fd = (int) sock;
	int cc;
	char bookings[1000];
	char readbuf[1000];
while(cc= read(fd, readbuf, 1000))
	{

if(strcmp(readbuf,"sanjose stockton\n")==0)
		{	
			printf("Cabs in the City details \n");
			fp=fopen("Cab_Display.txt","r");			
			bzero(bookings,1000);
			fread(bookings,1000,1,fp);
			write(fd,bookings,1000);
			fclose(fp);
			bzero(readbuf,1000);
		}
if(strcmp(readbuf,"b\n")==0)
		return;
		

	}
}

void handle_bookings(sqlite3* db,void* sock)
{	

	int fd = (int) sock;
	char bookings[1000];
	char readbuf[1000];
	char login1[1000];
	char from_to_flights[1000];
	int cc;
	book:
	fp=fopen("Password_authentication.txt","r");
	bzero(login1,1000);
	fread(login1,1000,1,fp);
	write(fd,login1,strlen(login1));
	fclose(fp);

	while(cc= read(fd, readbuf, 1000))
	{
		
				
		if (atoi(readbuf)==1)
		{	
		
			printf("please enter the details for the travel \n");
			fp=fopen("Flight_bookings.txt","r");
			bzero(bookings,1000);
			fread(bookings,1000,1,fp);
			write(fd,bookings,strlen(bookings));
			fclose(fp);
			bzero(readbuf,1000);
			handle_flight_bookings((void *) fd);
			goto book;
		}

		if (atoi(readbuf)==2)
		{	
			
                        customer_delete_database(db,fd);
			goto book;
			bzero(readbuf,1000);
		}

		if (atoi(readbuf)==3)
		{	
			printf("Book a hotel in the city\n");
			fp=fopen("Hotel_bookings.txt","r");
			bzero(bookings,1000);
			fread(bookings,1000,1,fp);
			write(fd,bookings,1000);
			fclose(fp);
			bzero(readbuf,1000);
			book_hotel((void *) fd);
			goto book;

		}

		if (atoi(readbuf)==4)
		{	
			printf("Book a Taxi to commute \n");
			fp=fopen("Taxi_bookings.txt","r");
			bzero(bookings,1000);
			fread(bookings,1000,1,fp);
			write(fd,bookings,1000);
			fclose(fp);
			bzero(readbuf,1000);
			book_a_cab((void *) fd);
			goto book;
		}

		if (atoi(readbuf)==5)
		{	
			
                      
                        customer_update_database(db,fd);
			bzero(readbuf,1000);
                        return;
		}

		if (atoi(readbuf)==6)
		{	
			printf("Contact us details \n");
			fp=fopen("contact_us.txt","r");			
			bzero(bookings,1000);
			fread(bookings,1000,1,fp);
			write(fd,bookings,1000);
			fclose(fp);
			bzero(readbuf,1000);
		}
			

		if(strcmp(readbuf,"b\n")==0)
		return;
		
			
	}

}
