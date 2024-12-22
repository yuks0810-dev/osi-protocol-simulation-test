#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    socklen_t addrlen = sizeof(address);
    char buffer[1024] = {0};

    // ソケットファイルディスクリプタの作成
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // ソケットオプションの設定 (アドレスをすぐに再利用できるように)
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt SO_REUSEADDR");
        exit(EXIT_FAILURE);
    }

    #ifdef SO_REUSEPORT
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)) < 0) {
        perror("setsockopt SO_REUSEPORT");
        exit(EXIT_FAILURE);
    }
    #endif

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; // 全てのアドレスでリッスン
    address.sin_port = htons(PORT);      // ポート番号をネットワークバイトオーダーに変換

    // ソケットをアドレスとポートにバインド
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // リッスン開始
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Listening on port %d...\n", PORT);

    while (1) {
        // クライアントからの接続を待機
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen)) < 0) {
            perror("accept");
            continue; // エラーが発生してもサーバーを停止せずに次の接続を待機
        }

        // クライアントからのメッセージを受信
        int bytes_read = read(new_socket, buffer, sizeof(buffer) - 1);
        if (bytes_read < 0) {
            perror("read");
            close(new_socket);
            continue;
        }

        buffer[bytes_read] = '\0';
        printf("Received: %s\n", buffer);

        // クライアントに応答を送信
        const char *message = "Hello from server! (this message is from server)";
        if (send(new_socket, message, strlen(message), 0) < 0) {
            perror("send");
        } else {
            printf("Message sent to client.\n");
        }

        // ソケットを閉じる
        close(new_socket);
    }

    // サーバーソケットを閉じる（実際には到達しませんが、念のため）
    close(server_fd);
    return 0;
}