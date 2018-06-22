#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string.h> 
#include<semaphore.h> 
extern sem_t mutex;
FILE *fp;
void agent_add_database(sqlite3*,int);
void agent_update_database(sqlite3*,int);
void agent_delete_database(sqlite3*,int);
void agent_display_database(sqlite3*,int);
void agent_create_database(sqlite3*,int);
int asprintf(char**,char*,...);
char send[1000];
int fd_p;
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


void airliner_database(int fd)
{

char* confirm_id="Authenticated!\n";
char options_buf[1000];


b:
fp=fopen("agent_options.txt","r");
fread(options_buf,1000,1,fp);
write(fd,options_buf,strlen(options_buf));
fclose(fp);



while(1)
{

/*----------------------------while was here-----------*/
int rr;
   sqlite3 *db;
   char *zErrMsg = 0;
   int  rc;
   char *input;
   char *name;
   char *sql;
   char readbuf[5000];
   const char* data = "Callback function called";
   /* Open database */
   rc = sqlite3_open("AIRLINER.db", &db);
   if( rc ){
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
 
   }else{
      fprintf(stdout, "Opened database successfully\n");
   }




rr=read(fd,readbuf,1000);
//printf("read= %s",readbuf);
if(strcmp(readbuf,"b\n")==0)
{

goto b;

}


  switch(atoi(readbuf)){
    case 0:
               printf("create a database table\n");
                 agent_create_database(db,fd);
                 break;
		

    case 1:
                printf("add to the data base\n");
                  agent_add_database(db,fd);
                  break;


    case 2:
                printf("update the data base\n");
                  agent_update_database(db,fd);
                break;
    case 3:
                printf("delete the data base\n");
                 agent_delete_database(db,fd);
                 break;

    case 4:   
               printf("display the data base\n");
                 agent_display_database(db,fd);
                break;

	
}
  bzero(readbuf,1000);
}
        
}

 void agent_add_database(sqlite3* db,int fd)
{
  
  
   char *zErrMsg = 0;
   int  rc;
   char *ft_no;
   char* seat;
   char *class;
   char* bag;
char readbuf[5000];
char* facilities;
sem_wait(&mutex); 
   char *sql;
   const char* data = "Callback function called";
const char* data1 = "Flight added successfully\n\n";
  

write(fd,"Enter the details in following format: <flight_no> <seats> <class> <baggage> <on board facilities>", 200);


bzero(readbuf,5000);
read(fd,readbuf,5000);


    //printf("value inside the name %s\n",readbuf);
     ft_no=strtok(readbuf," ");
     seat=strtok(NULL," ");
     class=strtok(NULL," ");
     bag=strtok(NULL," ");
facilities=strtok(NULL,"\n");


   asprintf(&sql,"INSERT INTO AIRLINER (FLIGHT_NO,SEATS,CLASS,BAGGAGE,ON_BOARD) VALUES('%s','%s','%s','%s','%s');",ft_no,seat,class,bag,facilities); 
   /* Execute SQL statement */
   rc = sqlite3_exec(db, sql, callback, (void*)data , &zErrMsg);
   if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s", zErrMsg);
      sqlite3_free(zErrMsg);
   }else{
      fprintf(stdout, "Flight added successfully\n");
write(fd,data1,strlen(data1));

   }
   bzero(readbuf,5000);
sem_post(&mutex);
   free(sql);
   sqlite3_close(db);
   return ;

}

 void agent_update_database(sqlite3* db,int fd)
{
  
 
   char *zErrMsg = 0;
   int  rc;
   char *sql;
   char *field;
   char *value;
   char* ft_no;
sem_wait(&mutex);
char readbuf[1000];
const char* data1 = "updated successfully\n\n";
   const char* data = "Callback function called";
 

write(fd,"Enter the details in following format: <flight_no> <field> <value>", 200);
bzero(readbuf,1000);
read(fd,readbuf,1000);


     ft_no=strtok(readbuf," ");
     field=strtok(NULL," ");
     value=strtok(NULL,"\n");

    asprintf(&sql,"UPDATE AIRLINER SET '%s' = '%s' WHERE FLIGHT_NO= '%s';",field,value,ft_no); 
   /* Execute SQL statement */
   rc = sqlite3_exec(db, sql, callback, (void*)data , &zErrMsg);
   if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s", zErrMsg);
      sqlite3_free(zErrMsg);
   }else{
      fprintf(stdout, "updated successfully\n\n");
write(fd,data1,strlen(data1));
 //bzero(readbuf,1000);
   }
sem_post(&mutex);
   sqlite3_close(db);
   return ;
}

void agent_delete_database(sqlite3* db,int fd)
{
  
  
   char *zErrMsg = 0;
   int  rc;
   char *sql;
 const char* data1= "deleted successfully\n\n";
   const char* data = "Callback function called";
char readbuf[1000];
char* buffer;
sem_wait(&mutex);

write(fd,"Enter in following format <flight_number>",50);
bzero(readbuf,1000);
read(fd,readbuf,1000);


buffer=strtok(readbuf,"\n");


   asprintf(&sql,"DELETE FROM AIRLINER WHERE FLIGHT_NO=\"%s\"",buffer);
   /* Execute SQL statement */
   
   rc = sqlite3_exec(db, sql, callback, (void*)data , &zErrMsg);
   if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   }else{
      fprintf(stdout, "deleted successfully\n\n");
 write(fd,data1,strlen(data1));
   }
   free(sql);
sem_post(&mutex);
   sqlite3_close(db);
   return ;
}

void agent_display_database(sqlite3* db,int fd)
{
  
   char *zErrMsg = 0;
   int  rc;
   char *sql;
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
      fprintf(stdout, "Operation done successfully\n\n");
   }
   sqlite3_close(db);
   return ;
}

void agent_create_database(sqlite3* db,int fd)
{

   char *zErrMsg = 0;
   int  rc;
   char *sql;
   const char* data = "Callback function called";
   sql= "CREATE TABLE AIRLINER(" \
         "FLIGHT_NO INT PRIMARY KEY," \
         "SEATS CHAR(50)," \
         "CLASS   CHAR(50)," \
         "BAGGAGE  CHAR(50)," \
         "ON_BOARD  CHAR(50));"; 
         
   /* Execute SQL statement */
   rc = sqlite3_exec(db, sql, callback, (void*)data , &zErrMsg);
   if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   }else{
      fprintf(stdout, "Database created successfully\n\n");
   }
   sqlite3_close(db);
   return ;

}











