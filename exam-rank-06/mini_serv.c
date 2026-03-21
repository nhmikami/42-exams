#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>

int		count = 0;
int		max_fd = 0;
int		ids[1024];
char	*msgs[1024];
char	buf_read[1024];
char	buf_write[42];
fd_set	rfds, wfds, afds; 
// conjunto de fds:
// - rfds: fds prontos para leitura (socket de escuta + clientes conectados)
// - wfds: fds prontos para escrita (clientes conectados)
// - afds: fds ativos (clientes conectados + socket de escuta)

int	extract_message(char **buf, char **msg) {
	char	*newbuf;
	int		i;

	*msg = 0;
	if (*buf == 0)
		return 0;

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

char	*str_join(char *buf, char *add) {
	char	*newbuf;
	int		len;

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

void	fatal_error() {
	write(2, "Fatal error\n", 12);
	exit(1);
}

void	notify_others(int fd, char *msg) {
	for (int i = 0; i <= max_fd; i++) {
		if (FD_ISSET(i, &wfds) && i != fd) // verifica se fd está ativo no conjunto
			send(i, msg, strlen	(msg), 0);
	}
}

void	send_msg(int fd) {
	char	*msg;

	while (extract_message(&msgs[fd], &msg) > 0) {
		sprintf(buf_write, "client %d: ", ids[fd]);
		notify_others(fd, buf_write);
		notify_others(fd, msg);
		free(msg);
	}
}

void	accept_client(int fd) {
	if (fd > max_fd)
		max_fd = fd;
	ids[fd] = count++;
	msgs[fd] = 0;
	FD_SET(fd, &afds); // adiciona o fd ao conjunto
	sprintf(buf_write, "server: client %d just arrived\n", ids[fd]);
	notify_others(fd, buf_write);
}

void	disconnect_client(int fd) {
	sprintf(buf_write, "server: client %d just left\n", ids[fd]);
	notify_others(fd, buf_write);
	free(msgs[fd]);
	msgs[fd] = 0;
	FD_CLR(fd, &afds); // remove o fd do conjunto
	close(fd);
}

int create_socket() {
	max_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (max_fd < 0)
		fatal_error();
	FD_SET(max_fd, &afds);
	return (max_fd);
}

int	main(int ac, char **av) {

	if (ac != 2) {
		write(2, "Wrong number of arguments\n", 26);
		exit(1);
	}

	FD_ZERO(&afds); // limpa o conjunto de fds
	int	sockfd = create_socket(); // socket de escuta do servidor

	struct sockaddr_in servaddr;
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(2130706433);
	servaddr.sin_port = htons(atoi(av[1]));	// replace 8081 with atoi(av[1])

	if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)))
		fatal_error();
	if (listen(sockfd, 10))
		fatal_error();

	while (42) {
		rfds = wfds = afds;
		if (select(max_fd + 1, &rfds, &wfds, NULL, NULL) < 0)
			fatal_error();

		for (int fd = 0; fd <= max_fd; fd++) {
			if (!FD_ISSET(fd, &rfds))
				continue;

			if (fd == sockfd) { // servidor
				socklen_t len = sizeof(servaddr);
				int client_fd = accept(sockfd, (struct sockaddr *)&servaddr, &len); // socket para o cliente 
				if (client_fd >= 0) {
					accept_client(client_fd);
					break;
				}
			} 
			else { // cliente
				int read_bytes = recv(fd, buf_read, 1000, 0);
				if (read_bytes <= 0) {
					disconnect_client(fd);
					break;
				}
				buf_read[read_bytes] = 0;
				msgs[fd] = str_join(msgs[fd], buf_read);
				send_msg(fd);
			}
		}
	}
	return 0;
}
