  #include <stdio.h>
  #include <sys/types.h>
  #include <sys/socket.h>
#include<time.h>
#include<pthread.h>
//#include<stdlib>
#include <unistd.h>
  #include <netinet/in.h>
#define NUM 100


//  #include <emscripten/emscripten.h>
  time_t cur_time;
  struct tm * time_info;
  char timeString[20];

  int sockfd, newsockfd, portno, clilen;
  char buffer[256];
  struct sockaddr_in serv_addr, cli_addr;
  int n;

  int *show_time()
  {
      time(&cur_time);
      time_info = localtime(&cur_time);

      strftime(timeString, sizeof(timeString),"%H:%M:%S",time_info);

//      printf("%s",timeString);

      n = write(newsockfd, timeString, 18);
      if (n < 0) error("ERROR writing to socket");

  }

  void error(char *msg)
  {
      perror(msg);
      exit(1);
  }


   int main(int argc, char *argv[])
   {        /*****time****/

        if (argc < 2) {
            fprintf(stderr,"ERROR, no port provided\n");
            exit(1);
        }
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0)
           error("ERROR opening socket");
        bzero((char *) &serv_addr, sizeof(serv_addr));
        portno = atoi(argv[1]);
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(portno);
        serv_addr.sin_addr.s_addr = INADDR_ANY;
        if (bind(sockfd, (struct sockaddr *) &serv_addr,
                 sizeof(serv_addr)) < 0)
                 error("ERROR on binding");
        listen(sockfd,5);
        clilen = sizeof(cli_addr);
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0)
             error("ERROR on accept");

/////////////////threads
       pthread_t th[NUM];
       int er;
       for(int i=0;i<NUM;i++) {

           er = pthread_create(&th[i],NULL,&show_time,NULL);

           sleep(1);
           //   clrscr();
           if(er)

           {
            //   std::cout << "err" << std::endl;
               exit(-1);
           }


       }
       //show_time();
       pthread_exit(NULL);
       ///////////threads



        //////write
       //bzero(buffer,256);
        //n = read(newsockfd,buffer,255);
        //if (n < 0) error("ERROR reading from socket");
       // printf("Here is the message: %s\n",buffer);



       return 0;
   }



// emcc -o SocketServer.html SocketsServer.c -s WASM=1 -O3 -s NO_EXIT_RUNTIME=1 -s EXPORTED_FUNCTIONS="['_main','_error']"