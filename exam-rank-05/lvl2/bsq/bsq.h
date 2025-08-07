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
void	print_map(t_map *map);
void	free_map(t_map *map);
size_t	ft_strlen(char *s);

t_map	*read_file(t_map *map, char *file);
void	process_input(t_map *map, char *file);

int		min3(int a, int b, int c);
void	solve_bsq(t_map *map);

#endif
