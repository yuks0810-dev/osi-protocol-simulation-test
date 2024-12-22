#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 80
#define HOST "www.jaist.ac.jp"
#define MAX_BUFFER 4096
#define DISPLAY_LENGTH 500 // 表示するHTMLの最大文字数

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[MAX_BUFFER] = {0};
    char initial_request[] = "GET / HTTP/1.1\r\nHost: www.jaist.ac.jp\r\nConnection: close\r\n\r\n";
    ssize_t bytes_received;
    int total_received = 0;

    // ソケットの作成
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        return -1;
    }

    // ホスト名の解決
    server = gethostbyname(HOST);
    if (server == NULL) {
        fprintf(stderr, "ERROR, no such host\n");
        close(sock);
        return -1;
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(PORT);

    // サーバーに接続
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection Failed");
        close(sock);
        return -1;
    }

    // HTTP GET リクエストを送信
    if (send(sock, initial_request, strlen(initial_request), 0) < 0) {
        perror("Send failed");
        close(sock);
        return -1;
    }
    printf("Initial HTTP request sent.\n");

    // レスポンスを受信して解析
    printf("Initial response from server:\n");
    memset(buffer, 0, MAX_BUFFER);
    if ((bytes_received = recv(sock, buffer, MAX_BUFFER - 1, 0)) > 0) {
        buffer[bytes_received] = '\0';
        printf("%s\n", buffer);

        // 302 Found の場合、Location ヘッダーを解析
        if (strncmp(buffer, "HTTP/1.1 302 Found", strlen("HTTP/1.1 302 Found")) == 0) {
            char *location_start = strstr(buffer, "Location: ");
            if (location_start != NULL) {
                location_start += strlen("Location: ");
                char *location_end = strchr(location_start, '\r');
                if (location_end != NULL) {
                    *location_end = '\0';
                    printf("Redirecting to: %s\n", location_start);

                    // ソケットを閉じる
                    close(sock);

                    // 新しいソケットを作成し、接続
                    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
                        perror("Socket creation error");
                        return -1;
                    }
                    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
                        perror("Connection Failed");
                        close(sock);
                        return -1;
                    }

                    // index.html にアクセスするための新しいリクエストを作成
                    char new_request[256];
                    snprintf(new_request, sizeof(new_request), "GET %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n", location_start, HOST);

                    // 新しいリクエストを送信
                    if (send(sock, new_request, strlen(new_request), 0) < 0) {
                        perror("Send failed");
                        close(sock);
                        return -1;
                    }
                    printf("New HTTP request sent for: %s\n", location_start);

                    // 新しいレスポンスを受信して表示
                    printf("Response from server for %s (first %d characters):\n", location_start, DISPLAY_LENGTH);
                    memset(buffer, 0, MAX_BUFFER);
                    total_received = 0;
                    while ((bytes_received = recv(sock, buffer, MAX_BUFFER - 1, 0)) > 0) {
                        buffer[bytes_received] = '\0';
                        int display_chars = (total_received + bytes_received > DISPLAY_LENGTH) ? (DISPLAY_LENGTH - total_received) : bytes_received;
                        printf("%.*s", display_chars, buffer);
                        total_received += bytes_received;
                        if (total_received >= DISPLAY_LENGTH) {
                            printf("\n... (truncated)\n");
                            break;
                        }
                    }
                    if (bytes_received < 0) {
                        perror("Receive failed");
                    }
                }
            }
        }
    } else if (bytes_received < 0) {
        perror("Receive failed");
    }

    // ソケットを閉じる
    close(sock);
    return 0;
}