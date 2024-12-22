#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8081
#define SERVER_IP "127.0.0.1"
#define NUM_MESSAGES 100 // 送信するメッセージの数

int main() {
    int sockfd;
    struct sockaddr_in servaddr;
    char message[50];

    // ソケットファイルディスクリプタの作成
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));

    servaddr.sin_family = AF_INET; // IPv4
    servaddr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, SERVER_IP, &servaddr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        close(sockfd);
        return -1;
    }

    // 高速にメッセージを送信
    for (int i = 1; i <= NUM_MESSAGES; i++) {
        snprintf(message, sizeof(message), "Message %d from client", i);
        if (sendto(sockfd, message, strlen(message), 0, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
            perror("sendto failed");
            continue;
        }
        printf("Sent: %s\n", message);
    }

    printf("All messages sent.\n");
    close(sockfd);
    return 0;
}