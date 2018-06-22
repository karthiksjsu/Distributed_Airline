#include<unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include<string.h> 
#include<semaphore.h> 
extern sem_t mutex;
void customer_display_database_booked(sqlite3* ,int );
void customer_add_database(sqlite3 * db,int);
void customer_update_database(sqlite3*,int);
void customer_delete_database(sqlite3*,int);
void customer_display_database(sqlite3*,int);
void customer_create_flightdatabase(int);
sqlite3 * customer_create_database(int);
char delete[100000];
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

void customer_add_database(sqlite3 * db,int fd)
{
  
   char *zErrMsg = 0;
   int  rc;
   char* name;
   char* sl_no;
   char* first_name;
   char* last_name;
   char* email_id;
   char* user_name;
   char* password;
   char* confirm_password;
   char  buf[10000];
   int cc;
sem_wait(&mutex);
   char *sql;
   const char* data = "Callback function called";
   const char* data1= "Operation done successfully\n";
  
     bzero(buf,10000);
    while( cc=read(fd,buf,10000))
{    

      //cc=read(fd,buf,10000);
      if(strcmp(buf,"b\n")==0)
	 return ;  
else
{

     //printf("value inside the name %s\n",buf);
     sl_no=strtok(buf," ");
     first_name=strtok(NULL," ");
     last_name=strtok(NULL," ");
     email_id=strtok(NULL," ");
     user_name=strtok(NULL," ");
     password=strtok(NULL," ");
     confirm_password=strtok(NULL,"\n");   
   
   printf("Token Values:'%d','%s','%s','%s','%s','%s','%s'\n",atoi(sl_no),first_name,last_name,email_id,user_name,password,confirm_password);
   asprintf(&sql,"INSERT INTO CUSTOMERS(SL_NO,FIRST_NAME,LAST_NAME,EMAIL_ID,USERNAME,PASSWORD,CONFIRM_PASSWORD) VALUES('%d','%s','%s','%s','%s','%s','%s');",atoi(sl_no),first_name,last_name,email_id,user_name,password,confirm_password); 

   /* Execute SQL statement */
   rc = sqlite3_exec(db, sql, callback, (void*)data , &zErrMsg);
   if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   }else{
      fprintf(stdout, "Operation done successfully\n");
      write(fd,data1,strlen(data1));
   }
   free(sql);
sem_post(&mutex);
   sqlite3_close(db);
   return;
 } 
}
}

void customer_update_database(sqlite3* db,int fd)
{
 
   char *zErrMsg = 0;
   int  rc;
   char *sql;
   char *field;
   char *value;
   char* sl_no;
   char buf[5000];
   int cc;
   char* name;
sem_wait(&mutex);
   const char* data1= "Update done successfully\n";
   const char* data = "Callback function called";


    

    const char *send_data="Enter the format in Which  <<sl_no>>  Which <<Field>> to be Updated to what <<Value>> ";
     write(fd,send_data,strlen(send_data));
      bzero(buf,1000);   
     cc=read(fd,buf,1000);    
     // printf("value inside the name %s\n",buf);
     sl_no=strtok(buf," ");
     field=strtok(NULL," ");
     value=strtok(NULL,"\n");
     //printf("values inside the read buf :%d %s %s\n", atoi(sl_no),field,value);
  
        printf("value of the db before %p\n",db);
  
    asprintf(&sql,"UPDATE CUSTOMERS SET '%s' = '%s' WHERE SL_NO=%d;",field,value,atoi(sl_no)); 

   /* Execute SQL statement */
   rc = sqlite3_exec(db, sql, callback, (void*)data , &zErrMsg);
   if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   }else{
      fprintf(stdout, "Update done successfully\n");
      write(fd,data1,strlen(data1));
   }
sem_post(&mutex);
   sqlite3_close(db);
   return ;
}


void customer_display_database(sqlite3* db,int fd)
{
  

	char *zErrMsg = 0;
	int  rc;
	char *sql;
	char* name;
	const char* data = "Callback function called";
	int cc;
	char * buff;
	char*buff1;
	char* buff2;
	char buf[1000]; 

        bzero(buf,1000);
   
   while(cc=read(fd,buf,1000))
{    
     
      if(strcmp(buf,"b\n")==0)
	 return ;                 
      else
	{
      buff=strtok(buf," ");
      buff1=strtok(NULL," ");
      buff2=strtok(NULL,"\n");
      printf("from:%s,to:%s,date:%s",buff,buff1,buff2);
   asprintf(&sql,"SELECT * from FLIGHTS where FROM_=\"%s\" and TO_=\"%s\" and DATE=\"%s\"",buff,buff1,buff2);

   /* Execute SQL statement */
   rc = sqlite3_exec(db, sql, callback, (void*)data , &zErrMsg);
   if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   }else{
        printf("value of send buffer %s",send);
       strcat(send," Select any one of the flights from the list with <<Sl_no>>\n\n");
       write(fd,send,strlen(send));
       customer_display_database_booked(db,fd);
       bzero(send,strlen(send));
      fprintf(stdout, "Operation done successfully\n");
   }
   sqlite3_close(db);
	bzero(buf,1000);
  }
	

}
}
void customer_display_database_booked(sqlite3* db,int fd)
{
  

   char *zErrMsg = 0;
   int  rc;

   char *sql;
   char* name;

   const char* data = "Callback function called";
    int cc;

    char * buff;
   char*buff1;
   char* buff2;
    char buf[1000]; 


     bzero(buf,1000);   
  cc=read(fd,buf,1000);
                   
    
   asprintf(&sql,"SELECT * from FLIGHTS where SL_NO=\"%d\"",atoi(buf));

   /* Execute SQL statement */
   rc = sqlite3_exec(db, sql, callback, (void*)data , &zErrMsg);
   if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   }else{
        printf("value of send buffer %s",send);
       strcpy(delete,send); 

      strcat(send,"Above flight is Booked successfully\n\n");
       write(fd,send,strlen(send));
            
       bzero(send,strlen(send));
      fprintf(stdout, "Operation done successfully\n");
system("python mail.py");
   }
   sqlite3_close(db);
	bzero(buf,1000);
  }
	
void customer_delete_database(sqlite3* db,int fd)
{
  
 
   char *zErrMsg = 0;
   int  rc;
   char *sql;
   char buf[5000];
   int cc;
   char *name;
   const char* data1= "Cancellation done successfully\n";
   const char* data = "Callback function called";
	printf("value of delete:%s\n",delete);
	const char *send_data="Booked flight details are as above\n\nSelect any one of the flights from the list with <<Sl_no>> to cancel ";
	strcat(delete,send_data);
	write(fd,delete,strlen(delete));
	bzero(buf,1000);   
	cc=read(fd,buf,1000);      
	write(fd,data1,strlen(data1));
	if(strcmp(buf,"b\n")==0)
	return;


}

void customer_create_flightdatabase(int fd)
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
   
  
   sql= "CREATE TABLE FLIGHTS(" \
         "SL_NO INT PRIMARY KEY," \
         "FROM_ CHAR(50)," \
         "TO_   CHAR(50)," \
         "DATE  CHAR(50)," \
         "AIRLINE  CHAR(50)," \
         "FLIGHT_NO CHAR(50)," \
         "TICKET_FAIR INT);";

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

sqlite3 * customer_create_database(int fd)
{
  

   sqlite3 *db;
   char *zErrMsg = 0;
   int  rc;
   char *sql;
   const char* data = "Callback function called";

   /* Open database */
   rc = sqlite3_open("mydb_Customer.db", &db);
   if( rc ){
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
   }else{
      fprintf(stdout, "Opened database successfully\n");
   }
   
  
   sql= "CREATE TABLE CUSTOMERS(" \
         "SL_NO INT PRIMARY KEY," \
         "FIRST_NAME CHAR(50)," \
         "LAST_NAME   CHAR(50)," \
         "EMAIL_ID  CHAR(50)," \
         "USERNAME  CHAR(50)," \
         "PASSWORD CHAR(50)," \
         "CONFIRM_PASSWORD CHAR(50));";

   /* Execute SQL statement */
   rc = sqlite3_exec(db, sql, callback, (void*)data , &zErrMsg);
   if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   }else{
      

      fprintf(stdout, "Operation done successfully\n");
   }
   sqlite3_close(db);
   return db;
}


		  


