// A simple server in the internet domain using TCP
// The port nu1mber is passed as an argument

// Please note this is a C program
// It compiles without warnings with gcc
#include <unistd.h>
#include <iostream>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <math.h>
#include <deque>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <iostream>
#include <sys/wait.h>
using namespace std;
// CONVERTS CHAR TO STRING
string char2str(char buffer[256]){
    string str = "";
    int i =0;
    while(buffer[i]){
        str +=buffer[i];
        i++;
    }
    return str;
}

//CONVERTS TO INT
int convertToInt (string s){
    int i;
    string ss;
    if (sscanf(s.c_str(), "%d", &i) == 1) {
        return i;
    } else {
        cout<<"could not convert"<<s<<endl;
        return -1;
    }
}
//GETS THE SIZE OF THE BITS
string sizeOfBits(string binary, int bits){
    //cout<<"1 binary:"<<binary<<" bits"<<bits<<" binary.size():"<<binary.size()<<endl;
    if(binary.size()==1)
        return binary;
    else{
        for(int i =0;i<bits;i++){
            binary = "0"+binary;
        }
        return binary;
    }
}
//CONVERTS TO BINARY
string int2Binary(int n)
{
    string binary = "";
    int aar[32];
    int i = 0;
    while (n > 0) {
        aar[i] = n % 2;
        n = n / 2;
        i++;
    }
    for (int j = i - 1; j >= 0; j--){
        binary += to_string(aar[j]);
    }
    return binary;
}//FUNCTION THAT CREATESS ELIAS GAMMA CODE
string eliasGammaCode(string str)
{
    string binary = int2Binary(convertToInt(str));
    int bitsSize = log2(convertToInt(str));
    string str1= sizeOfBits(binary, bitsSize);
    return str1;
}
void error(char *msg)
{
    perror(msg);
    exit(1);
}
void fireman(int)
{
    while (waitpid(-1, NULL, WNOHANG) > 0)
        std::cout << "A child process ended" << std::endl;
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno, clilen;
     char buffer[256];
     struct sockaddr_in serv_addr, cli_addr;
     int n;
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
    //ccreates socket to connect wit client and reminds open for new msgs
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0)
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
    //listens for new msgs
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
    int m= 0;
    //fireman function
   // cout<<" parent process started" <<endl;
   
    signal(SIGCHLD,fireman);
    while(1){
         newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, (socklen_t *)&clilen);
        if (fork()==0)
        {
           // cout<< pid<<" A child process started" <<endl;
            //creates new socket and acceepts msg
           
         //   cout<< pid<<"------------------- A child process about to end" <<endl;
            if (newsockfd < 0)
                error("ERROR on accept");
            bzero(buffer,256);
            n = read(newsockfd,buffer,256);
            if (n < 0) error
                ("ERROR reading from socket");
            // this where i get binary and eliasGamma code
            string number = char2str(buffer);
            string msg = int2Binary(convertToInt(number))+","+eliasGammaCode(number);
            int sizeChar = msg.size();
            char buffer3 [256];
            int i =0;
            //convert string to char array
            while(msg.size()>i ) {
                buffer3[i]=msg[i];
                i++;
            }
            //writting into the socket and returning msg
           n = write(newsockfd,buffer3,sizeChar);
           if (n < 0)
               error("ERROR writing to socket");
           
            //exiting current child process
            //cout<< pid<<"------------------- A child process about to end" <<endl;
            close(newsockfd);
          _exit(0);
        }
        //cout<<" parent process ended" <<endl;
        m++;
        close(newsockfd);
        //sleep(1);
        
        //sleep(2);
    }
    close(sockfd);
    return 0;
}

