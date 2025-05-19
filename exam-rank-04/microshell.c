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

int	exec_cmd(char **av, int i, char **ev, int *fd_bk)
{
	int	pid;
	int	fd[2];
	int	status = 0;
	int	has_pipe = 0;

	if (av[i] && !strcmp(av[i], "|"))
		has_pipe = 1;
	if (has_pipe && pipe(fd) == -1)
		return (ft_error("error: fatal\n"));

	pid = fork();
	if (pid == -1)
		return (ft_error("error: fatal\n"));

	if (pid == 0)
	{
		av[i] = NULL;
		if (dup2(*fd_bk, 0) == -1)
			return (ft_error("error: fatal\n"));
		close(*fd_bk);
		if (has_pipe)
		{
			if (dup2(fd[1], 1) == -1)
				return (ft_error("error: fatal\n"));
			close(fd[0]);
			close(fd[1]);
		}
		execve(av[0], av, ev);
		ft_error("error: cannot execute ");
		ft_error(av[0]);
		ft_error("\n");
		exit(1);
	}

	if (!has_pipe) {
		if (dup2(0, *fd_bk) == -1)
			return (ft_error("error: fatal\n"));
	}
	else
	{
		close(fd[1]);
		close(*fd_bk);
		*fd_bk = fd[0];
	}
	if (waitpid(pid, &status, 0) == -1)
		return (ft_error("error: fatal\n"));
    return (status);
}

int	main(int ac, char **av, char **ev)
{
	(void)ac;
	int	i = 1;
	int	res = 0;
	int	fd_bk = dup(0);
	
	if (fd_bk < 0)
		ft_error("error: fatal\n");
	while (av[i])
	{
		av = av + i;
		i = 0;
		while (av[i] && strcmp(av[i], "|") && strcmp(av[i], ";"))
			i++;
		if (!strcmp(av[0], "cd"))
			res = exec_cd(av, i);
		else
			res = exec_cmd(av, i, ev, &fd_bk);
		if (av[i])
			i++;
	}
	close(fd_bk);
	return (res);
}

// valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all --track-fds=yes --trace-children=yes ./a.out  /bin/ls | /bin/grep micro