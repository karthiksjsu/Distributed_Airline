#include<unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include<string.h>
#include<semaphore.h> 
void add_database(sqlite3*,int);
void update_database(sqlite3*,int);
void delete_database(sqlite3*,int);
void display_database(sqlite3*,int);
void create_database(sqlite3*,int);
int asprintf(char**,char*,...);
int fd_p;
char send[100000];
extern sem_t mutex;

static int callback(void *data, int argc, char **argv, char **azColName){
   int i;
   int fd=fd_p;
   

   for(i=0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
       strcat(send,azColName[i]);
       strcat(send,"=");
       strcat(send,argv[i]);
       strcat(send,"\n"); 

   }
      strcat(send,"\n");
  
   printf("\n");
   return 0;
}

void add_database(sqlite3* db,int fd)
{
  
 
   char *zErrMsg = 0;
   int  rc;
    char* name;
   char *sl_no;
   char *from;
   char *to;
   char* date;
   char*airline;
   char*flight_no;
   char* seat_no;
   char* fare;
   char buf[5000];
   int cc;
 sem_wait(&mutex);
   char *sql;
   const char* data = "Callback function called";
   const char* data1= "Flight added successfully\n";
    const char *send_data="Enter the format in  <<sl_no>>  <<From>>  <<To>>  <<Date>>  <<Airlines>>  <<Flight_No>> <<No_of_Seats>> <<Fare>> to add";
     write(fd,send_data,strlen(send_data));
     cc=read(fd,buf,1000);    
    //printf("value inside the name %s\n",buf);
     sl_no=strtok(buf," ");
     from=strtok(NULL," ");
     to=strtok(NULL," ");
     date=strtok(NULL," ");
     airline=strtok(NULL," ");
     flight_no=strtok(NULL," ");
     seat_no=strtok(NULL," ");
      fare=strtok(NULL,"\n");
   asprintf(&sql,"INSERT INTO FLIGHTS (SL_NO,FROM_,TO_,DATE,AIRLINE,FLIGHT_NO,NUMBER_OF_SEATS,TICKET_FARE) VALUES('%d','%s','%s','%s','%s','%s','%s','%s');",atoi(sl_no),from,to,date,airline,flight_no,seat_no,fare); 
   /* Execute SQL statement */
   rc = sqlite3_exec(db, sql, callback, (void*)data , &zErrMsg);
   if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   }else{
      fprintf(stdout, "Flight added successfully\n");
      write(fd,data1,strlen(data1));
   }
   free(sql);
   sem_post(&mutex);
   sqlite3_close(db);
   return ;

}

void update_database(sqlite3* db,int fd)
{
  
 
   char *zErrMsg = 0;
   int  rc;
   char *sql;
   char *field;
   char *value;
   char* sl_no;
   char buf[5000];
   int cc;
sem_wait(&mutex);
   char* name;
   const char* data1= "Flight detail updated successfully\n";
   const char* data = "Callback function called";
 
    const char *send_data="Enter the format in Which  <<sl_no>>  Which <<Field>> to be Updated to what <<Value>> ";
     write(fd,send_data,strlen(send_data));
      bzero(buf,1000);   
     cc=read(fd,buf,1000);    
      //printf("value inside the name %s\n",buf);
     sl_no=strtok(buf," ");
     field=strtok(NULL," ");
     value=strtok(NULL," ");
    asprintf(&sql,"UPDATE FLIGHTS SET '%s' = '%s' WHERE SL_NO= \"%d\";",field,value,atoi(sl_no)); 
   /* Execute SQL statement */
   rc = sqlite3_exec(db, sql, callback, (void*)data , &zErrMsg);
   if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   }else{
      fprintf(stdout, "Flight detail updated successfully\n");
      write(fd,data1,strlen(data1));
   }
sem_post(&mutex);
   sqlite3_close(db);
   return ;
}

void delete_database(sqlite3* db,int fd)
{
  
  
   char *zErrMsg = 0;
   int  rc;
 
   char *sql;
   char buf[5000];
   int cc;
   char *name;
sem_wait(&mutex);
   const char* data1= "Flight deleted successfully\n";
   const char* data = "Callback function called";
  
    const char *send_data="Enter the format in Which  <<sl_no>>  Needs to be deleted ";
     write(fd,send_data,strlen(send_data));
     bzero(buf,1000);   
     cc=read(fd,buf,1000);    
   asprintf(&sql,"DELETE FROM FLIGHTS WHERE SL_NO=\"%d\"",atoi(buf));
   /* Execute SQL statement */
  
   rc = sqlite3_exec(db, sql, callback, (void*)data , &zErrMsg);
   if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   }else{
      fprintf(stdout, "Flight deleted successfully\n");
      write(fd,data1,strlen(data1));
   }
sem_post(&mutex);
   free(sql);
   sqlite3_close(db);
   return ;
}

void display_database(sqlite3* db,int fd)
{
  

   char *zErrMsg = 0;
   int  rc;
 
   char *sql;
   char* name;
   fd_p=fd;
   const char* data = "Callback function called";
   sql ="SELECT * from FLIGHTS";

   /* Execute SQL statement */
   rc = sqlite3_exec(db, sql, callback, (void*)data , &zErrMsg);
   if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   }else{
   
       write(fd,send,strlen(send));
       bzero(send,strlen(send));
      fprintf(stdout, "Operation done successfully\n");
   }
   sqlite3_close(db);
   return ;
}

void create_database(sqlite3* db,int fd)
{
  
  
   char *zErrMsg = 0;
   int  rc;
  
   char *sql;
   const char* data = "Callback function called";
   sql= "CREATE TABLE FLIGHTS(" \
         "SL_NO INT PRIMARY KEY," \
         "FROM_ CHAR(50)," \
         "TO_   CHAR(50)," \
         "DATE  CHAR(50)," \
         "AIRLINE  CHAR(50)," \
         "FLIGHT_NO CHAR(50)," \
         "NUMBER_OF_SEATS  CHAR(50)," \
         "TICKET_FARE  CHAR(50));";
   /* Execute SQL statement */
   rc = sqlite3_exec(db, sql, callback, (void*)data , &zErrMsg);
   if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   }else{
      

      fprintf(stdout, "Operation done successfully\n");
   }
   sqlite3_close(db);
   return ;
}

		  


