# include <unistd.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <string.h> 

int	err(char *str)
{
	while (*str)
		write(2, str++, 1);
	return 1;
}

int	exec_cd(char **av, int i)
{
	if (i != 2)
		return err("error: cd: bad arguments\n");
	else if (chdir(av[1]) == -1)
		return err("error: cd: cannot change directory to "), err(av[1]), err("\n");
	return 0;
}

int	exec_cmd(char **av, char **ev, int i)
{
	int	fd[2];
	int	status;
	int	has_pipe = av[i] && !strcmp(av[i], "|");

	if (has_pipe && pipe(fd) == -1)
		return err("error: fatal\n");

	int pid = fork();
	if (!pid)
	{
		av[i] = 0;
		if (has_pipe && (dup2(fd[1], 1) == -1 || close(fd[0]) == -1 || close(fd[1]) == -1))
			err("error: fatal\n"), exit(1);
		execve(*av, av, ev);
		err("error: cannot execute "), err(*av), err("\n"), exit(1);
	}
	if (waitpid(pid, &status, 0) == -1)
		return err("error: fatal\n");
	if (has_pipe && (dup2(fd[0], 0) == -1 || close(fd[0]) == -1 || close(fd[1]) == -1))
		return err("error: fatal\n");
	if (WIFEXITED(status))
		return WEXITSTATUS(status);
	return 1;
}

int	main(int ac, char **av, char **ev)
{
	int	i = 1;
	int	status = 0;
	int	fd_stdin = dup(0);
	if (fd_stdin == -1)
		return err("error: fatal\n");

	if (ac > 1)
	{
		while (av[i])
		{
			av += i;
			i = 0;
			while (av[i] && strcmp(av[i], "|") && strcmp(av[i], ";"))
				i++;
			if (!strcmp(av[0], "cd"))
				status = exec_cd(av, i);
			else
				status = exec_cmd(av, ev, i);
			if (!av[i] || !strcmp(av[i], ";"))
			{
				if (dup2(fd_stdin, 0) == -1)
					return err("error: fatal\n");
			}
			if (av[i])
				i++;
		}
	}
	close(fd_stdin);
	return status;
}
