#include "bsq.h"

// utils
void	print_error(void) {
	fprintf(stderr, "map error\n");
}

void	print_map(t_map *map) {
	if (!map || !map->grid)
		return ;

	for (int i = 0; i < map->rows; i++) {
		fprintf(stdout, "%s", map->grid[i]);
		fprintf(stdout, "\n");
	}
}

void	free_map(t_map *map) {
	for (int i = 0; i < map->rows; i++) {
		if (map->grid[i])
			free(map->grid[i]);
	}
	if (map->grid)
		free(map->grid);
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
t_map	*read_file(char *file, t_map *map) {
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

	if (file)
		fclose(fp);

	for (int i = 0; i < map->rows; i++) {
		for (int j = 0; j < map->cols; j++) {
			char c = map->grid[i][j];
			if (c != map->empty && c != map->obstacle)
				return (NULL);
		}
	}
	return (map);
}

void	process_input(char *file, t_map *map) {
	if (read_file(file, map) != NULL) {
		solve_bsq(map);
		print_map(map);
	}
	else
		print_error();
	free_map(map);
}

// solve
int	min_sqr(int a, int b, int c) {
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
	
	int	max_size = 0;
	int	max_row = 0;
	int	max_col = 0;
	for (int r = 0; r < map->rows; r++) {
		for (int c = 0; c < map->cols; c++) {
			if (map->grid[r][c] == map->empty) {
				if (r == 0 || c == 0)
					dp[r][c] = 1;
				else
					dp[r][c] = min_sqr(dp[r - 1][c], dp[r][c - 1], dp[r - 1][c - 1]) + 1;
				if (dp[r][c] > max_size) {
					max_size = dp[r][c];
					max_row = r;
					max_col = c;
				}
			}
		}
	}

	for (int r = max_row - max_size + 1; r <= max_row; r++) {
		for (int c = max_col - max_size + 1; c <= max_col; c++) {
			map->grid[r][c] = map->full;
		}
	}

	for (int i = 0; i < map->rows; i++)
		free(dp[i]);
	free(dp);
}

// main
int	main(int ac, char **av) {
	t_map	*map = calloc(1, sizeof(t_map));
	if (!map) {
		print_error();
		return (1);
	}

	if (ac < 2)
		process_input(NULL, map);
	else {
		int i = 1;
		while (i < ac) {
			if (i > 1)
				fprintf(stdout, "\n");
			process_input(av[i], map);
			i++;
		}
	}
	free(map);
	return (0);
}

// cc -Wall -Werror -Wextra bsq.c
