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

void send_data(int sock, char *text);
char *get_data();

int main(int argc, char *argv[]) {
    char text[128];
    char rtext[128];
    int sock, csock;
    struct sockaddr_in svr;
    struct sockaddr_in clt;
    struct hostent *cp;
    int clen;
    char rbuf[1024];
    int nbytes;
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
        send_data(sock, text);

        /* データを受信 */
        bzero(rtext, 128);
	if(read(sock, rtext, strlen(rtext))<0){
		perror("get_data");
		exit(1);
	}

        /* 文字列を出力 */
    	write(1, rtext, strlen(rtext));
   }

    close(sock);
    printf("closed\n");
}

/* データ送出 */
void send_data(int sock, char *text) {
    int text_len;

    text_len = sizeof(text);
    write(sock, text, text_len);
}

/* データ受信 */
char* get_data(int sock) {
    char* rtext;
    if (read(sock, rtext, sizeof(rtext)) < 0){
        perror("get_data");
        exit(1);
    }
    return rtext;
}
