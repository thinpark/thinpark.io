#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>

int main(void)
{
        int len;
        int result;
        int sockfd;
        char ch = 'A';
        struct sockaddr_in address;

        sockfd = socket(AF_INET, SOCK_STREAM, 0);

        address.sin_family = AF_INET;
        address.sin_addr.s_addr = inet_addr("127.0.0.1");
        address.sin_port = htons(9734);
        len = sizeof(address);

        result = connect(sockfd, (struct sockaddr *)&address, len);
        if (result == -1) {
                perror("oops: client");
                exit(1);
        }

        write(sockfd, &ch, 1);
        read(sockfd, &ch, 1);
        printf("%d :- char from server = %c\n", getpid(), ch);
        close(sockfd);

        exit(0);
}
