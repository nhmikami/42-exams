#include "bsq.h"

// cc -Wall -Werror -Wextra bsq.c

// utils
void	print_map(t_map *map) {
	if (!map || !map->grid)
		return ;
	for (int i = 0; i < map->rows; i++)
		fprintf(stdout, "%s\n", map->grid[i]);
}

void	free_map(t_map *map) {
	for (int i = 0; i < map->rows; i++) {
		if (map->grid && map->grid[i])
			free(map->grid[i]);
	}
	if (map->grid) {
		free(map->grid);
		map->grid = NULL;
	}
}

size_t	ft_strlen(char *s) {
	size_t len = 0;
	while (s[len] != '\0' && s[len] != '\n')
		len++;
	return (len);
}

int	is_printable(char c) {
	return (c >= 32 && c <= 126);
}

// input
t_map	*read_file(t_map *map, char *file) {
	FILE	*fp = file ? fopen(file, "r") : stdin;
	if (!fp)
		return (NULL);
	
	if (fscanf(fp, "%d %c %c %c\n", &map->rows, &map->empty, &map->obstacle, &map->full) != 4) {
		if (file) fclose(fp);
		return (NULL);
	}
	if (map->rows <= 0 || map->empty == map->obstacle || map->empty == map->full || map->obstacle == map->full
		|| !is_printable(map->empty) || !is_printable(map->obstacle) || !is_printable(map->full)) {
		if (file) fclose(fp);
		return (NULL);
	}

	map->cols = 0;
	map->grid = calloc(map->rows, sizeof(char *));
	for (int i = 0; i < map->rows; i++) {
		char	*line = NULL;
		size_t	len = 0;
		if (getline(&line, &len, fp) == -1) {
			if (file) fclose(fp);
			return (NULL);
		}

		int	line_len = ft_strlen(line);
		if (line[line_len] == '\n')
			line[line_len] = '\0';
		if (i == 0)
			map->cols = line_len;
		else if (line_len != map->cols) {
			free(line);
			if (file) fclose(fp);
			return (NULL);
		}

		for (int j = 0; j < map->cols; j++) {
			if (line[j] != map->empty && line[j] != map->obstacle) {
				free(line);
				if (file) fclose(fp);
				return (NULL);
			}
		}
		map->grid[i] = line;
	}

	char *extra = NULL;
	size_t len = 0;
	if (getline(&extra, &len, fp) != -1) {
		free(extra);
		if (file) fclose(fp);
		return (NULL);
	}
	free(extra);
	if (file) fclose(fp);
	return (map);
}

void	process_input(t_map *map, char *file) {
	if (read_file(map, file) != NULL) {
		solve_bsq(map);
		print_map(map);
	}
	else
		fprintf(stderr, "map error\n");
	free_map(map);
}

// solve
int	min3(int a, int b, int c) {
	if (a <= b && a <= c)
		return a;
	if (b <= a && b <= c)
		return b;
	return c; 
}

void	solve_bsq(t_map *map) {
	int	**dp = calloc(map->rows, sizeof(int *));
	for (int i = 0; i < map->rows; i++)
		dp[i] = calloc(map->cols, sizeof(int));
	
	int	max = 0;
	int	max_row = 0;
	int	max_col = 0;
	for (int r = 0; r < map->rows; r++) {
		for (int c = 0; c < map->cols; c++) {
			if (map->grid[r][c] == map->empty) {
				if (r == 0 || c == 0)
					dp[r][c] = 1;
				else
					dp[r][c] = min3(dp[r - 1][c], dp[r][c - 1], dp[r - 1][c - 1]) + 1;
				if (dp[r][c] > max) {
					max = dp[r][c];
					max_row = r;
					max_col = c;
				}
			}
		}
	}

	for (int r = max_row - max + 1; r <= max_row; r++) {
		for (int c = max_col - max + 1; c <= max_col; c++)
			map->grid[r][c] = map->full;
	}

	for (int i = 0; i < map->rows; i++)
		free(dp[i]);
	free(dp);
}

// main
int	main(int ac, char **av) {
	t_map	*map = calloc(1, sizeof(t_map));
	if (!map) {
		fprintf(stderr, "map error\n");
		return (1);
	}

	if (ac < 2)
		process_input(map, NULL);
	else {
		for (int i = 1; i < ac; i++) {
			if (i > 1)
				fprintf(stdout, "\n");
			process_input(map, av[i]);
		}
	}
	free(map);
	return (0);
}
