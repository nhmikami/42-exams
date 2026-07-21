#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>

int		sockfd;
int		count = 0;
int		max_fd = 0;
int		ids[1024];
char	*inbox[1024];	// incoming messages
char	*outbox[1024];	// outgoing messages
char	read_buf[1024];
char	write_buf[42];
fd_set	afds, rfds, wfds;
// fds set:
// - afds: active fds (server socket + connected clients)
// - rfds: fds ready to read
// - wfds: fds ready to write


int extract_message(char **buf, char **msg) {
	char *newbuf;
	int i;

	*msg = 0;
	if (*buf == 0)
		return (0);

	i = 0;
	while ((*buf)[i]) {
		if ((*buf)[i] == '\n') {
			newbuf = calloc(1, sizeof(*newbuf) * (strlen(*buf + i + 1) + 1));
			if (newbuf == 0)
				return (-1);
			strcpy(newbuf, *buf + i + 1);
			*msg = *buf;
			(*msg)[i + 1] = 0;
			*buf = newbuf;
			return (1);
		}
		i++;
	}
	return (0);
}

char *str_join(char *buf, char *add) {
	char *newbuf;
	int len;

	if (buf == 0)
		len = 0;
	else
		len = strlen(buf);

	newbuf = malloc(sizeof(*newbuf) * (len + strlen(add) + 1));
	if (newbuf == 0)
		return (0);
	newbuf[0] = 0;
	if (buf != 0)
		strcat(newbuf, buf);
	free(buf);
	strcat(newbuf, add);
	return (newbuf);
}

void fatal_error(void) {
	write(2, "Fatal error\n", 12);
	exit(1);
}

void notify_others(int fd, char *msg) {
	for (int i = 0; i <= max_fd; i++) {
		if (FD_ISSET(i, &afds) && i != fd && i != sockfd) {
			outbox[i] = str_join(outbox[i], msg);
			if (!outbox[i])
				fatal_error();
		}
	}
}

void send_msg(int fd) {
	char *msg;
	int ret;

	while (ret = extract_message(&inbox[fd], &msg) > 0) {
		sprintf(write_buf, "client %d: ", ids[fd]);
		notify_others(fd, write_buf);
		notify_others(fd, msg);
		free(msg);
	}
	if (ret < 0)
		fatal_error();
}

void flush_client(int fd) {
	if (!outbox[fd] || !outbox[fd][0])
		return ;

	char *rest;
	int len = strlen(outbox[fd]);
	int sent = send(fd, outbox[fd], len, 0);
	if (sent > 0) {
		rest = malloc(len - sent + 1);
		if (!rest)
			fatal_error();
		strcpy(rest, outbox[fd] + sent);
		free(outbox[fd]);
		outbox[fd] = rest;
	}
}

void accept_client(int fd) {
	if (fd > max_fd)
		max_fd = fd;
	ids[fd] = count++;
	inbox[fd] = 0;
	outbox[fd] = 0;
	FD_SET(fd, &afds);
	sprintf(write_buf, "server: client %d just arrived\n", ids[fd]);
	notify_others(fd, write_buf);
}

void disconnect_client(int fd) {
	sprintf(write_buf, "server: client %d just left\n", ids[fd]);
	FD_CLR(fd, &afds);
	notify_others(fd, write_buf);
	free(inbox[fd]);
	inbox[fd] = 0;
	free(outbox[fd]);
	outbox[fd] = 0;
	close(fd);
}

int create_socket(void) {
	max_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (max_fd < 0)
		fatal_error();
	FD_SET(max_fd, &afds);
	return (max_fd);
}

int main(int ac, char **av) {

	if (ac != 2) {
		write(2, "Wrong number of arguments\n", 26);
		exit(1);
	}

	FD_ZERO(&afds);
	sockfd = create_socket();	// server socket

	struct sockaddr_in servaddr;
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;					// IPv4
	servaddr.sin_addr.s_addr = htonl(2130706433);	// 127.0.0.1
	servaddr.sin_port = htons(atoi(av[1]));			// port - received from command line argument

	if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) != 0)
		fatal_error();
	if (listen(sockfd, 10) != 0)
		fatal_error();

	while (42) {
		rfds = afds;
		FD_ZERO(&wfds);

		for (int fd = 0; fd <= max_fd; fd++) {
			if (outbox[fd] && outbox[fd][0])
				FD_SET(fd, &wfds);
		}

		if (select(max_fd + 1, &rfds, &wfds, NULL, NULL) < 0)
			fatal_error();

		for (int fd = 0; fd <= max_fd; fd++) {
			if (fd == sockfd && FD_ISSET(fd, &rfds)) {	// server
				struct sockaddr_in cliaddr;
				socklen_t len = sizeof(cliaddr);
				int connfd = accept(sockfd, (struct sockaddr *)&cliaddr, &len);	// client socket
				if (connfd >= 0)
					accept_client(connfd);
				continue;
			}

			if (FD_ISSET(fd, &wfds))	// ready to write
				flush_client(fd);

			if (FD_ISSET(fd, &rfds)) {	// ready to read
				int bytes_read = recv(fd, read_buf, 1000, 0);
				if (bytes_read <= 0) {
					disconnect_client(fd);
					continue;
				}
				read_buf[bytes_read] = 0;
				inbox[fd] = str_join(inbox[fd], read_buf);
				if (!inbox[fd])
					fatal_error();
				send_msg(fd);
			}
		}
	}
	return (0);
}
