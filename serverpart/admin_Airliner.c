#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include<string.h> 
#include<semaphore.h> 
extern sem_t mutex;
ssize_t read(int,void*, size_t);
int asprintf(char**,char*,...);
int fd_p;
char send[100000];

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

void add_Airliner_database(sqlite3* db,int fd)
{
  
 
   char *zErrMsg = 0;
   int  rc;
   char* password;
   char*Airliner_name;
   char*username;
   char buf[5000];
   int cc;
  sem_wait(&mutex);
   char *sql;
   const char* data = "Callback function called";
   const char* data1= "Airliner added successfully\n";
    const char *send_data="Enter the format in  <<Airliner>>  <<Username>>  <<Password>>  to add";
     write(fd,send_data,strlen(send_data));
     cc=read(fd,buf,1000);    
    //printf("value inside the name %s\n",buf);
     Airliner_name=strtok(buf," ");
     username=strtok(NULL," ");
     password=strtok(NULL," ");
  
   
   asprintf(&sql,"INSERT INTO AIRLINER (AIRLINER_NAME,USERNAME,PASSWORD) VALUES('%s','%s','%s');",Airliner_name,username,password); 
   /* Execute SQL statement */
   rc = sqlite3_exec(db, sql, callback, (void*)data , &zErrMsg);
   if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   }else{
      fprintf(stdout, "Airliner added successfully\n");
      write(fd,data1,strlen(data1));
   }
   free(sql);
sem_post(&mutex);
   sqlite3_close(db);
   return ;

}
void delete_Airliner_database(sqlite3* db,int fd)
{
  
  
   char *zErrMsg = 0;
   int  rc;
 
   char *sql;
   char *buff;
   char buf[5000];
   int cc;
   char *name;
sem_wait(&mutex);
   const char* data1= "Airliner deleted successfully\n";
   const char* data = "Callback function called";

    const char *send_data="Enter the format in Which  <<Airliner>>  Needs to be deleted ";
     write(fd,send_data,strlen(send_data));
     bzero(buf,1000);   
     cc=read(fd,buf,1000);    
      buff=strtok(buf,"\n");
   asprintf(&sql,"DELETE FROM AIRLINER WHERE AIRLINER_NAME=\"%s\"",buff);
   /* Execute SQL statement */
   
   rc = sqlite3_exec(db, sql, callback, (void*)data , &zErrMsg);
   if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   }else{
      fprintf(stdout, "Airliner deleted successfully\n");
      write(fd,data1,strlen(data1));
   }
   free(sql);
sem_post(&mutex);
   sqlite3_close(db);
   return ;
}

void display_Airliner_database(sqlite3* db,int fd)
{
  

   char *zErrMsg = 0;
   int  rc;
  
   char *sql;
   char* name;
   fd_p=fd;
   const char* data = "Callback function called";
   sql ="SELECT * from AIRLINER";

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

void create_Airliner_database(sqlite3* db,int fd)
{
  
 
   char *zErrMsg = 0;
   int  rc;

   char *sql;
   const char* data = "Callback function called";
   const char* data1= "Operation done successfully\n";
   sql= "CREATE TABLE AIRLINER(" \
         "AIRLINER_NAME CHAR(50) PRIMARY KEY," \
         "USERNAME CHAR(50)," \
         "PASSWORD  CHAR(50));";
   /* Execute SQL statement */
   rc = sqlite3_exec(db, sql, callback, (void*)data , &zErrMsg);
   if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   }else{
      

      fprintf(stdout, "Operation done successfully\n");
      write(fd,data1,strlen(data1));
   }
   sqlite3_close(db);
   return ;
}

		  


