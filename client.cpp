
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <math.h>
#include <deque>
using namespace std;
struct values
{
    char buffer[256];
    string val;
    string msg;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    int portno;
    string binary;
    string eliasGammaCode;
};
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
//HELPER
deque <string> back(deque <string> l){
    if(l.empty()){
        return l;
    }
    else{
        l.erase(l.begin());
        return l;
    }
}
void error(char *msg)
{
    perror(msg);
    exit(0);
}
//CONVERTS CHAR TO STRING
string char2str(char buffer[256]){
    string str = "";
    int i =0;
    while(buffer[i]){
        str +=buffer[i];
        i++;
    }
    return str;
}
//GETS ONLY THE BINARY PART FROM MSG
string leftbinary(string str){
    string binary = "";
    int i =0;
    while(str[i] != ','){
        binary +=str[i];
        i++;
    }
    return binary;
}
//GETS ONLY THE ELIAS GAMMA CODE PART FROM MSG
string rightGammaCode(string str){
    string gammaCode = "";
    int i;
    for(int j =0; j<str.size();j++){
        if(str[j]==','){
            i=j+1;
            while(i<str.size()){
                gammaCode +=str[i];
                i++;
            }
            break;
        }
    }
    return gammaCode;
}
// FUNCTTION FOR THREADS
void * send2server(void * struct_void){
    struct values *numbers = (struct values *)struct_void;
    int sockfd, portno, n;
    struct hostent *server= numbers->server;
    char buffer[256];
    struct sockaddr_in serv_addr;
    portno =numbers->portno;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    // server = gethostbyname();
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR connecting");
    n = write(sockfd,numbers->buffer,numbers->val.size());
    if (n < 0)
        error("ERROR writing to socket");
    bzero(buffer,256);
    n = read(sockfd,buffer,256);
    if (n < 0)
        error("ERROR reading from socket idk");
    //printf("%s\n",buffer);

    numbers->msg = char2str(buffer);
}
int main(int argc, char *argv[])
{
    // struct sockaddr_in serv_addr;
    char buffer[256];
    if (argc < 3) {
        fprintf(stderr,"usage %s hostname port\n", argv[0]);
        exit(0);
    }
    //READS FILE
    string line;
    deque<string> lines;
    while(cin>>line){
        lines.push_back(line);
    }
    string num = lines.front();
    int size = convertToInt(num);
    struct values numbers[size];
    lines =back(lines);
    pthread_t tid[size];
    string argv0 = argv[0];
    string argv1 = argv[1];
    for(int i=0;i<size;i++){
        numbers[i].portno= atoi(argv[2]);
        numbers[i].server=gethostbyname(argv[1]);
        string num = lines[i];
        numbers[i].val="";
        int j=0;
        while (num.size() > j) {
            numbers[i].buffer[j] = num[j];
            numbers[i].val += num[j];
            j++;
        }
        if(pthread_create(&tid[i], NULL, send2server, &numbers[i]))
        {
            fprintf(stderr, "Error creating thread\n");
            return 1;
        }
    }
// JOINS THE THREADS
    for (int i = 0; i < size; i++)
        pthread_join(tid[i], NULL);
    // PARENT PROCESS PRINTS THE CODES
    for(int i =0; i<size;i++) {
        cout<<"Value: "<< numbers[i].val << ", Binary Code: "<<leftbinary(numbers[i].msg)<<", Elias-Gamma code: "<<rightGammaCode(numbers[i].msg)<< endl;
    }
    return 0;
}
