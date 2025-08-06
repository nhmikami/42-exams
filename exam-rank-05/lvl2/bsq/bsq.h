#ifndef BSQ_H
#define BSQ_H

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct s_map {
	int		rows;
	int		cols;
	char	empty;
	char	full;
	char	obstacle;
	char	**grid;
}	t_map;

int		is_printable(char c);
void	print_error(void);
void	print_map(t_map *map);
void	free_map(t_map *map);
size_t	ft_strlen(char *s);

t_map	*read_file(char *file, t_map *map);
void	process_input(char *file, t_map *map);

int		min_sqr(int a, int b, int c);
void	solve_bsq(t_map *map);

#endif
