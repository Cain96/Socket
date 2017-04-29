#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>

#define STRLEN 128

int main(int argc, char *argv[]) {
    char text[STRLEN];
    char *rtext;
    int sock;
    struct sockaddr_in svr;
    char rbuf[1024];
    int reuse;
    struct hostent *host;

    /* 引数が1つであることの確認 */
    if (argc != 2) {
        perror("Illegal arguments");
        exit(1);
    }

    /* ソケットの生成 */
    if ((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        perror("socket");
        exit(1);
    }
    /* ソケットアドレス再利用の指定 */
    reuse = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
		perror("setsockopt");
		exit(1);
	    }

	    memset((char *) &svr, 0, sizeof(svr));
	    /* client_addr構造体にサーバの設定 */
	    if((host = gethostbyname(argv[1])) == NULL){
		perror("setIP");
		exit(1);
	    }
	    bcopy(host->h_addr, &svr.sin_addr, host->h_length);
    svr.sin_family = AF_INET;
    svr.sin_port = htons(10120);

    /* connectでhostと接続 */
    if (connect(sock, (struct sockaddr *) &svr, sizeof(svr)) > 0) {
        perror("connect");
        close(sock);
        exit(1);
    }

    /*文字列をキーボードから入力*/
    while (scanf("%s", text) != EOF) {
        
        /* データを送出 */
        int text_len = sizeof(text);
        write(sock, text, text_len);

        /* データを受信 */
	if(read(sock, rtext, STRLEN)<0){
		perror("get_data");
		exit(1);
	}

        /* 文字列を出力 */
	printf("%s\n", rtext);
   }

    close(sock);
    printf("closed\n");
}

