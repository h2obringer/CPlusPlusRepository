#include <sys/types.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <sys/errno.h>
#include <netinet/in.h>
#include <string.h>

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include "libnet.h"

#define QLEN               5    /* maximum connection queue length      */
#define BUFSIZE         4096
#define LINELEN         256

/* local routines */
static int reaper(void);
static int TCPechod(int fd,char* pword);

/* globals */
static int debug = 1;

/*------------------------------------------------------------------------
 * main - Concurrent TCP server for ECHO service
 *------------------------------------------------------------------------
 */
int main(argc, argv) int     argc; char    *argv[];{
  char    *service = "5000";      /* service name or port number  */
  struct  sockaddr_in fsin;       /* the address of a client      */
  u_int   alen;                   /* length of client's address   */
  int     msock;                  /* master server socket         */
  int     ssock;                  /* slave server socket          */
  char password[LINELEN+1];


        switch (argc) {
        case    1:
                break;
        case    2:
                service = argv[1];
                break;
        default:
                Error("usage: TCPechod [port]\n");
        }

	/* if the port is "-", use a random port */
	if (strcmp(service,"-") == 0)
	    service = NULL;

        fprintf(stdout,"Enter a password for all incoming connections:\n");
        if (fgets((char*) password, 2048, stdin) == NULL)
          exit(0);
        
        /* remove the newline */
        password[strlen(password)-1] = '\00';
        fprintf(stdout,"Your password is: %s\n",password);

        msock = passiveTCP(service, QLEN);

	printf("Server running on local port %u\n", lport(msock));

        signal(SIGCHLD, (void (*)())reaper);

        while (1) {
                alen = sizeof(fsin);
                ssock = accept(msock, (struct sockaddr *)&fsin, &alen);
                if (ssock < 0) {
                        if (errno == EINTR)
                                continue;
                        Error("accept: %s\n", strerror(errno));
                }
                switch (fork()) {
                case 0:         /* child */
                        (void) close(msock);
                        exit(TCPechod(ssock,(char*)password));
                default:        /* parent */
                        (void) close(ssock);
                        break;
                case -1:
                        Error("fork: %s\n", strerror(errno));
                }
        }
}


/*------------------------------------------------------------------------
 * TCPechod - echo data until end of file
 *------------------------------------------------------------------------
 */
static int TCPechod(int fd,char* pword){
  int j;
  char buf[BUFSIZ];
  int cc;
  FILE *f;
  char *cmd;
  char *pch;
  char line[256];
  char compare[BUFSIZ];
  char confirmation[BUFSIZ];
  compare[0]='N';
  compare[1]='O';
  compare[2]='\00';
  confirmation[0]='N';
  confirmation[1]='O';
  confirmation[2]='\00';
  while(1){
    fprintf(stdout,"compare string = %s before reading it\n",compare);
    cc=read(fd,compare,sizeof(compare)); /*read in the password*/
    decrypt_data(compare); /*decrypt the password*/
    for(j=0;compare[j]!=NULL;++j){ /*NULL terminate the password*/
      if((compare[j]=='\r')||(compare[j]=='\n')){
        compare[j]='\00';
      }
    }
    if(strcmp(pword,compare)==0){
      fprintf(stdout,"Send YES!\n");
      confirmation[0]='Y';
      confirmation[1]='E';
      confirmation[2]='S';
      confirmation[3]='\r';
      confirmation[4]='\n';
      confirmation[5]='\00';
      encrypt_data(confirmation);
      write(fd, (char*)confirmation, strlen(confirmation));
      break;
    }
  }

  cc = read(fd, buf, sizeof buf);
  fprintf(stdout,"before decrypting command: %s\n",buf);
  decrypt_data(buf);
  fprintf(stdout,"after decrypting command: %s\n", buf);
	
    
  if (cc < 0)
  {
      Error("myssh read: %s\n", strerror(errno));
  } 
  else if( cc != 0 ) 
  {
    cmd = malloc(strlen(buf+10));
    for(j=0;buf[j]!=NULL;++j){
      if(buf[j]=='\r'){
        fprintf(stdout,"Found it");
        buf[j]='\00';
        break;
      }
    }

    sprintf(cmd,"/bin/%s\n", buf);

    f = popen(cmd, "r");

    while ((fgets(line,sizeof(line),f)) != NULL) {
	if ((pch = strchr(line,'\n')) != NULL) 
	    *pch = '\00';  /* nuke the newline*/
	puts(line); /*display on the server side before adding official end of line characters*/
        strncat(line,"\r\n",sizeof(line));/*add end of line characters*/ /*was sizeof buf*/
        fprintf(stdout,"before encrypting: %s\n",line);
        encrypt_data(line);
        fprintf(stdout,"after encrypting: %s\n",line);
        write(fd, line, strlen(line));/*send them to the host*/
    }

    pclose(f);
    close(fd);
    fprintf(stdout,"closed the file descriptor\n");
  }
  
  
  return 0;
}

/*------------------------------------------------------------------------
 * reaper - clean up zombie children
 *------------------------------------------------------------------------
 */
static int
reaper(void)
{
    int status;
    int pid;
    pid_t wait3(int *statusp, int options, struct rusage *rusage);

    while ((pid=wait3(&status, WNOHANG, NULL)) > 0) {
	if (debug)
	    printf("process %d reaped\n", pid);
	/* empty */;
    }

    /* re-install myself for the next child */
    signal(SIGCHLD, (void (*)())reaper);

    return(0);
}
