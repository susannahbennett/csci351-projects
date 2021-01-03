#include <stdio.h>
#include "csapp.h"

// Susannah Bennett

/* Recommended max cache and object sizes */
#define MAX_CACHE_SIZE 1049000
#define MAX_OBJECT_SIZE 102400

static const char *user_agent_hdr = "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3\r\n";

typedef struct {
  char cache_obj[MAX_OBJECT_SIZE]; /* stores server response */
  char cache_url[MAXLINE]; /* contains port, path, and hostname */
  int counter; /* counts how recently used it is */
  int valid;

} cache_block;

cache_block *myCache;
pthread_rwlock_t lock;
int lruTime = 0; /* time stamp */

void *thread(void *vargp);
void doit(int connfd);
void initCache();
int checkCache(char *url);
void addToCache (char *url, char *store);

int main(int argc,char **argv) {

  initCache();
  int listenfd,connfd;
  socklen_t clientlen;
  char hostname[MAXLINE],port[MAXLINE];
  pthread_t tid;

  struct sockaddr_storage clientaddr;

  /* Error handling */
  if(argc != 2){
    fprintf(stderr,"usage :%s <port> \n",argv[0]);
    exit(1);
  }

  Signal(SIGPIPE, SIG_IGN); /*proxy ignores SIGPIPE signals */

   listenfd = Open_listenfd(argv[1]);

   for(;;){
      /* Sets up the proxy to accept incoming connections */
       clientlen = sizeof(clientaddr);
       connfd = Accept(listenfd,(SA*)&clientaddr,&clientlen);

      /*print accepted message*/
       Getnameinfo((SA*)&clientaddr,clientlen,hostname,MAXLINE,port,MAXLINE,0);
       printf("Accepted connection from (%s %s).\n",hostname,port);

       Pthread_create(&tid,NULL,thread,(void *)connfd);
   }

   free(myCache);

   return 0;
}

/* For functionality of threads */
void *thread(void *arg){
   int connfd = (int)arg;

   Pthread_detach(pthread_self());

   doit(connfd);
   Close(connfd);
}

/*Read and parse requests */
void doit(int connfd) {

  /*The end server file descriptor*/
  int end_serverfd; 

  char buf[MAXLINE],method[MAXLINE],url[MAXLINE];
  char http_header[MAXLINE];

  /*store the request line arguments*/
  char hostname[MAXLINE],path[MAXLINE];

  char *port; /* Port number */

  /* rio is client and server_rio is server */
  rio_t rio, server_rio;

  /* Initializing the client to listen to the file descriptor connfd */
  Rio_readinitb(&rio,connfd);
  /* Reading from client into the buffer */
  Rio_readlineb(&rio,buf,MAXLINE);

  /* Read the client request line */
  sscanf(buf, "%s http://%[^/]%s HTTP%*4c", method, hostname, path);

  /* For parsing for url */
  char extra[MAXLINE];
  sscanf(buf, "%s %s %s", extra, url, extra);

  /* If method is not equal to "GET" can't complete request */
  if(strcasecmp(method,"GET")) {
    printf("Proxy does not implement the method");
    return;
  }

  /* For finding the port number */
  port = strchr(hostname, ':');
  
  if (port != NULL) {
    *port++ = '\0';
  } else {
    port = "80"; // Web server
  }

  /* Before connecting to server, check if web object is in cache */
  pthread_rwlock_wrlock(&lock);
  int index = checkCache(url);
  pthread_rwlock_unlock(&lock);

  if (index >= 0) { // returns index if success or -1 if not in cache
    /* if a cache hit */
    pthread_rwlock_rdlock(&lock);
    rio_writen(connfd, myCache[index].cache_obj, MAX_OBJECT_SIZE); // reading from cache to client
    pthread_rwlock_unlock(&lock);
    return;
  }

  char request_hdr[MAXLINE],other_hdr[MAXLINE],host_hdr[MAXLINE];

   /* creates first request line */
  sprintf(request_hdr,"GET %s HTTP/1.0\r\n",path);

  /* Get other request header for client rio and change it */
  while(Rio_readlineb(&rio,buf,MAXLINE)>0) {
    /* EOF */
    if(strcmp(buf,"\r\n")==0) break;
    /* Host */
    if(!strncasecmp(buf,"Host",strlen("Host"))) {
      strcpy(host_hdr,buf);
      continue;
    }
    if(!strncasecmp(buf,"Connection",strlen("Connection"))
      &&!strncasecmp(buf,"Proxy-Connection",strlen("Proxy-Connection"))
      &&!strncasecmp(buf,"User-Agent",strlen("User-Agent"))) {
      
      strcat(other_hdr,buf);
    }
  }

  if(strlen(host_hdr)==0) {
    sprintf(host_hdr,"Host: %s\r\n",hostname);
  }

  /* Client to server format */
  sprintf(http_header,"%s%s%s%s%s%s%s",
          request_hdr,
          host_hdr,
          "Connection: close\r\n",
          "Proxy-Connection: close\r\n",
          user_agent_hdr,
          other_hdr,
          "\r\n");

  Signal(SIGPIPE, SIG_IGN); /*proxy ignores SIGPIPE signals */

  end_serverfd = Open_clientfd(hostname,port); // changed from port

  /* Error checking with opening file */
  if(end_serverfd<0) {
    printf("connection failed\n");
    return;
  }

  Rio_readinitb(&server_rio,end_serverfd);

  /* Write the http header to endserver */
  Rio_writen(end_serverfd,http_header,strlen(http_header));

  /* Receive message from end server and write to the client */
  size_t n;
  int storeSize = 0;
  char store[MAX_OBJECT_SIZE];

  while((n=Rio_readlineb(&server_rio,buf,MAXLINE))!=0) {
    storeSize+=n;
    if (storeSize < MAX_OBJECT_SIZE) {
      /* doesn't consider a string */
      // memcpy(&store[storeSize], buf, n); /* doesn't work */
      strcat(store, buf);
    }
    /* Sending server's response to the client */
    Rio_writen(connfd,buf,n);
  }

  if (storeSize < MAX_OBJECT_SIZE) {

    pthread_rwlock_wrlock(&lock);
    // Add to cache method
    addToCache(url, store);

    pthread_rwlock_unlock(&lock);
  }

  Close(end_serverfd);
}

void initCache() {

  myCache = calloc(10, sizeof(cache_block)); /* makes space for 10 cache_blocks */
  int i;
  for(i = 0; i < 10; i++) {
    myCache[i].counter = 0;
    myCache[i].valid = 0;
  }
}

int checkCache(char *url) {

  int k;
  for (k = 0; k < 10; k++) {
    if ((!strcmp(myCache[k].cache_url, url)) && (myCache[k].valid)) {
      // Add a clock stamp to the line (determining how recently it was used)
      myCache[k].counter = lruTime++; 
      return k; // in case that it is there
    }
  }
  return -1; // isn't there
}

/* Function called if not in cache already, will add to cache */
void addToCache (char *url, char *store) {
  // Find the LRU line, by finding the minimum value for counter
  int m;
  int minIndex = 0;
  int min = myCache[0].counter; /* smallest time stamp value */

  for (m = 1; m < 10; m++){
    if ((myCache[m].counter) < min) {
        min = myCache[m].counter;
        minIndex = m;
    }
  }

  /* Change the LRU to be the given entry */
  myCache[minIndex].valid = 1;
  strcpy(myCache[minIndex].cache_url, url);
  myCache[minIndex].counter = lruTime++;
  strcpy(myCache[minIndex].cache_obj, store);
}