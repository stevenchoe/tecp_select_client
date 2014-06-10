#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define BUFSIZE 1024
void error_exit(char *message);

int main(int argc, char **argv) {
	int client_fd;
	char message[BUFSIZE];
	int str_len, ret;
	struct sockaddr_in server_addr;
	fd_set fs_status;

	if (argc != 3) {
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	client_fd = socket(PF_INET, SOCK_STREAM, 0);
	if (client_fd == -1)
		error_exit("client_fdet() error");

	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(argv[1]);
	server_addr.sin_port = htons(atoi(argv[2]));

	ret = connect(client_fd, (struct sockaddr*) &server_addr,
			sizeof(server_addr));
	if (ret == -1)
		error_exit("connect() error!");

	printf("client_fd = %d\n", client_fd);

	while (1) {
		FD_ZERO(&fs_status);
		FD_SET(0, &fs_status);
		FD_SET(client_fd, &fs_status);

		select(client_fd+1, &fs_status, 0, 0, 0);

		if (FD_ISSET(0, &fs_status) == 1) {
			str_len = read(0, message, BUFSIZE);
			write(client_fd, message, str_len);
			if (message[0] == 'q')
			{
				close(client_fd);
				return 0;
			}
		} else if (FD_ISSET(client_fd, &fs_status)) {
			str_len = read(client_fd, message, BUFSIZE);
			printf("[server : ");
			fflush(stdout);
			write(1, message, str_len-1);
			printf("]\n");
		}

	}

	return 0;
}

void error_exit(char *message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
