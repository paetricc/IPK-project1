#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

#define BUFFSIZE 512

unsigned long long runCommand(char *cmd, char *output) {
    FILE *file = NULL;
    file = popen(cmd, "r");
    if(!file){
        perror("Popen function call fail\n");
        exit(EXIT_FAILURE);
    }
    fgets(output, BUFFSIZE, file);
    if(pclose(file) != 0) {
        perror("Pclose function call fail\n");
        exit(EXIT_FAILURE);
    }
    return atoll(output);
}

unsigned long long getCPUData(unsigned long long *Idle) {
    char tmp[BUFFSIZE];
    long long user, nice, system, idle, iowait, irq, softirq, steal;
    user = runCommand("cat /proc/stat | grep -w 'cpu' | awk -F \" \" '{print $2}'", tmp);
    nice = runCommand("cat /proc/stat | grep -w 'cpu' | awk -F \" \" '{print $3}'", tmp);
    system = runCommand("cat /proc/stat | grep -w 'cpu' | awk -F \" \" '{print $4}'", tmp);
    idle = runCommand("cat /proc/stat | grep -w 'cpu' | awk -F \" \" '{print $5}'", tmp);
    iowait = runCommand("cat /proc/stat | grep -w 'cpu' | awk -F \" \" '{print $6}'", tmp);
    irq = runCommand("cat /proc/stat | grep -w 'cpu' | awk -F \" \" '{print $7}'", tmp);
    softirq = runCommand("cat /proc/stat | grep -w 'cpu' | awk -F \" \" '{print $8}'", tmp);
    steal = runCommand("cat /proc/stat | grep -w 'cpu' | awk -F \" \" '{print $9}'", tmp);

    *(Idle) = idle + iowait;

    return ((*Idle) + (user + nice + system + irq + softirq + steal));
}

double getCPUUsage() {
    unsigned long long prevIdle, idle, prevTotal, Total;
    double totald, idled;
    prevTotal = getCPUData(&prevIdle);
    sleep(1);
    Total = getCPUData(&idle);

    totald = Total - prevTotal;
    idled = idle - prevIdle; 

    return ((totald - idled) / totald)*100;
}

int main(int argc, const char * argv[]) {
    char output[BUFFSIZE];
    int server_socket, optval, rc;
    if(argc != 2) {
        fprintf(stderr, "Wrong parametr\n");
        exit(EXIT_FAILURE);
    }
    int portNumber = atoi(argv[1]);
    // TODO check if correct port number

    if((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket function call fail\n");
        exit(EXIT_FAILURE);
    }
    printf("CPU USAGE: %f%%\n", getCPUUsage());
    return 0;

    optval = 1;
    if(setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, (const void*)&optval, sizeof(int))  == -1) {
        perror("Setsockopt function call fail\n");
        exit(EXIT_FAILURE);
    }
    if(setsockopt(server_socket, SOL_SOCKET, SO_REUSEPORT, (const void*)&optval, sizeof(int))  == -1) {
        perror("Setsockopt function call fail\n");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in serverAddress;
    bzero((char *) &serverAddress, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    printf("%d\n", portNumber);
    serverAddress.sin_port = htons((unsigned short)portNumber);

    if((rc = bind(server_socket, (struct sockaddr *) &serverAddress, sizeof(serverAddress))) < 0) {
        perror("Bind function call fail\n");
        exit(EXIT_FAILURE);
    }

    if((listen(server_socket, 1)) == - 1) {
        perror("Listen function call fail\n");
        exit(EXIT_FAILURE);
    }

    while(1) {
        int comm_socket = accept(server_socket, NULL, NULL);
        printf("cus\n");
        close(comm_socket);
    }

    runCommand("cat /proc/sys/kernel/hostname", output);
    printf("Hostname: %s", output);
    runCommand("cat /proc/cpuinfo | grep \"model name\" | uniq | awk  -F \":\" '{print $2}' | sed 's/ //'", output);
    printf("CPU name: %s", output);

    return 0;
}