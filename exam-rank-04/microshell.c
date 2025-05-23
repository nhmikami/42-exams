#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int	ft_strlen(char *str)
{
	int	len = 0;

	while (str[len])
		len++;
	return (len);
}

int	ft_error(char *str)
{
	write(2, str, ft_strlen(str));
	return (1);
}

int exec_cd(char **av, int i)
{
	if (i != 2)
		return (ft_error("error: cd: bad arguments\n"));
	if (chdir(av[1]) == -1)
	{
		ft_error("error: cd: cannot change directory to ");
		ft_error(av[1]);
		ft_error("\n");
		return (1);
	}
	return (0);
}

int	exec_cmd(char **av, int i, char **ev, int *fd_in)
{
	int	pid;
	int	pipe_fd[2];
	int	status = 0;
	int	has_pipe = 0;

	// verifica se há pipe
	if (av[i] && !strcmp(av[i], "|"))
		has_pipe = 1;
	if (has_pipe && pipe(pipe_fd) == -1)
		return (ft_error("error: fatal\n"));

	// cria o processo filho
	pid = fork();
	if (pid == -1)
		return (ft_error("error: fatal\n"));

	// processo filho
	if (pid == 0) {
		av[i] = NULL;
		// redireciona a entrada padrão para fd_in
		if (dup2(*fd_in, 0) == -1) {
			ft_error("error: fatal\n");
			exit (1);
		}
		close(*fd_in);

		if (has_pipe) {
			// redireciona a saída padrão para o lado de escrita do pipe
			if (dup2(pipe_fd[1], 1) == -1) {
				close(pipe_fd[0]);
				close(pipe_fd[1]);
				ft_error("error: fatal\n");
				exit (1);
			}
			close(pipe_fd[0]);
			close(pipe_fd[1]);
		}
		// executa o comando
		execve(av[0], av, ev);
		ft_error("error: cannot execute ");
		ft_error(av[0]);
		ft_error("\n");
		exit(127);
	}

	// processo pai
	if (has_pipe) {
		// redireciona o fd_in para o lado de leitura do pipe
		if (dup2(pipe_fd[0], *fd_in) == -1)
			return (ft_error("error: fatal\n"));
		close(pipe_fd[1]);
		close(pipe_fd[0]);
	}
	else {
		// redireciona o fd_in para a entrada padrão
		if (dup2(0, *fd_in) == -1)
			return (ft_error("error: fatal\n"));
	}

	if (waitpid(pid, &status, 0) == -1)
		return (ft_error("error: fatal\n"));
    if (WIFEXITED(status))
		return WEXITSTATUS(status);
	return (1);
}

int	main(int ac, char **av, char **ev)
{
	(void)ac;
	int	i = 1;
	int	status = 0;
	int	fd_in = dup(0);
	
	if (fd_in < 0)
		return (ft_error("error: fatal\n"));
	while (av[i])
	{
		av = av + i;
		i = 0;
		while (av[i] && strcmp(av[i], "|") && strcmp(av[i], ";"))
			i++;
		if (!strcmp(av[0], "cd"))
			status = exec_cd(av, i);
		else
			status = exec_cmd(av, i, ev, &fd_in);
		if (av[i])
			i++;
	}
	close(fd_in);
	return (status);
}

// valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all --track-fds=yes ./a.out  /bin/ls | /bin/grep micro