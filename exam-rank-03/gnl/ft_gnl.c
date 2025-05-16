#include <stdlib.h>
#include <unistd.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1
# endif

char	*ft_strchr(char *s, int c)
{
	while (*s)
	{
		if (*s == c)
			return (s);
		s++;
	}
	if (*s == c)
		return (s);
	return (NULL);
}

int	ft_strlen(char *s)
{
	int	len = 0;

	while (s && s[len])
		len++;
	return (len);
}

void	ft_strcpy(char *dst, char *src)
{
	int i = 0;

	if (!dst || !src)
		return ;
	while (src && src[i])
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
}

char	*ft_strdup(char *src)
{
	if (!src)
		return (NULL);
	char *dst = malloc(sizeof(char) * (ft_strlen(src) + 1));
	if (!dst)
		return (NULL);
	ft_strcpy(dst, src);
	return (dst);
}

char	*ft_strjoin(char *s1, char *s2)
{
	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
		return (ft_strdup(s1));
	int	len1 = ft_strlen(s1);
	int	len2 = ft_strlen(s2);
	char *join = malloc(sizeof(char) * (len1 + len2 + 1));
	if (!join)
		return (NULL);
	ft_strcpy(join, s1);
	ft_strcpy(join + len1, s2);
	free(s1);
	return (join);
}

static char	*read_until_nl(int fd, char *line, char *buffer)
{
	int	bytes_read;

	bytes_read = 1;
	while (bytes_read > 0)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read < 0)
			return (NULL) ;
		if (bytes_read == 0)
			break ;
		buffer[bytes_read] = '\0';
		line = ft_strjoin(line, buffer);
		if (!line)
			return (NULL);
		if (ft_strchr(line, '\n'))
			break ;
	}
	return (line);
}

char	*get_next_line(int fd)
{
	char		*line;
	char		*backup;
	static char	buffer[BUFFER_SIZE + 1];

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	line = ft_strdup(buffer);
	line = read_until_nl(fd, line, buffer);
	if (!line || line[0] == '\0')
	{
		if (line)
			free(line);
		return (NULL);
	}
	backup = ft_strchr(line, '\n');
	if (backup)
	{
		ft_strcpy(buffer, backup + 1);
		backup[1] = '\0';
	}
	else
		buffer[0] = '\0';
	return (line);
}

#include <fcntl.h>
#include <stdio.h>
int main(int ac, char **av)
{
	int     fd;
	char    *line;

	if (ac < 2)
	{
		printf("Missing file\n");
		return (1);
	}
	fd = open(av[1], O_RDONLY);
	if (fd < 0)
	{
		printf("Error opening file\n");
		return (1);
	}
	while ((line = get_next_line(fd)) != NULL)
	{
		printf("%s", line);
		free(line);
	}
	close(fd);
	return (0);
}
