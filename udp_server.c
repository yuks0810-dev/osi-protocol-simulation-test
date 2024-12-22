#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 8081
#define PROCESSING_DELAY_MS 100 // メッセージ処理の遅延時間（ミリ秒）

int main() {
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;
    char buffer[1024];
    
    // ソケットファイルディスクリプタの作成
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    servaddr.sin_family = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    // ソケットをアドレスとポートにバインド
    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("UDP Server listening on port %d\n", PORT);

    while (1) {
        socklen_t len = sizeof(cliaddr);
        int n;

        // データを受信
        n = recvfrom(sockfd, (char *)buffer, sizeof(buffer) - 1, 0, (struct sockaddr *)&cliaddr, &len);
        if (n < 0) {
            perror("recvfrom failed");
            continue; // エラーが発生してもサーバーを停止せずに次のメッセージを待機
        }

        buffer[n] = '\0';
        printf("Received: %s from %s:%d\n", buffer, inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));

        // 処理の遅延をシミュレーション
        usleep(PROCESSING_DELAY_MS * 1000); // マイクロ秒単位

        // メッセージを受信したことを確認（実際のACKは送信しない）
        // ドロップを確認するためにサーバーのみでログを確認します
    }

    // ソケットを閉じる（実際には無限ループのため到達しません）
    close(sockfd);
    return 0;
}