/* TCPecho.c - main, TCPecho */
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include<errno.h>

extern int errno;
int TCPecho(const char *host, const char *service);
int errexit(const char *format, ...);
int connectTCP(const char *host, const char *service);
#define LINELEN 128

/*------------------------------------------------------------------------
 * main - TCP client for ECHO service
 *------------------------------------------------------------------------
 */
FILE *fp;

int main(int argc, char *argv[])
{
	char *host = "localhost";
	char *service = "echo";
	switch (argc) {
		case 1:
			host = "localhost";
			break;
		case 3:
			service = argv[2];

		case 2:
			host = argv[1];
			break;
		default:
			fprintf(stderr, "usage: TCPecho [host [port]]\n");
			exit(1);
	}
	TCPecho(host, service);
	exit(1);
	return 0;
}

/*------------------------------------------------------------------------
 * TCPecho - send input to ECHO service on specified host and print reply
 *------------------------------------------------------------------------
 */
int TCPecho(const char *host, const char *service)
{
	int s, n,num;
	char read1[1000];
        char read2[1000];
	int outchars, inchars;
        char buf[10000];
	/* characters sent and received */
	s = connectTCP(host, service);
	
         n = read(s, read1,1000);
		if (n < 0)
			errexit("socket read failed: %s\n",strerror(errno));
        printf("%s\n",read1);
 
       printf("%s",">>");      
       while (fgets(buf,sizeof(buf),stdin))
 {
      // fgets(buf, sizeof(buf), stdin);
       buf[strlen(buf)+1] = '\0';


      outchars = strlen(buf);

           
      (void) write(s, buf,outchars);
         bzero(read2,1000);
        n=read(s,read2,1000);
         if(n<0)
        printf("error");
       if(atoi(read2)==10)
              exit(0);
      else {
        printf("%s\n",read2);
        printf("%s",">>");
         }



 }

return 0;
}

