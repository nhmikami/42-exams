#include "life.h"

// cc -Wall -Werror -Wextra life.c

// utils
int	**create_board(int w, int h) {
	int	**b = calloc(h, sizeof(int *));
	for (int i = 0; i < h; i++)
		b[i] = calloc(w, sizeof(int));
	return (b);
}

void	free_board(int **b, int h) {
	for (int i = 0; i < h; i++) {
		if (b[i])
			free(b[i]);
	}
	if (b)
		free(b);
}

void	print_board(int **b, int w, int h) {
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			if (b[i][j])
				putchar('0');
			else
				putchar(' ');
		}
		putchar('\n');
	}
}

// board
void	read_input(int **b, int w, int h) {
	int		x = 0;
	int		y = 0;
	int		draw = 0;
	char	c;

	while (read(0, &c, 1) > 0) {
		if (c == 'x')
			draw = !draw;
		else if (c == 'w' && y > 0)
			y--;
		else if (c == 'a' && x > 0)
			x--;
		else if (c == 's' && y < h - 1)
			y++;
		else if (c == 'd' && x < w - 1)
			x++;
		if (draw)
			b[y][x] = 1;
	}
}

int	**update_board(int **b, int w, int h) {
	int	**new = create_board(w, h);
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			int n = count_neighbors(b, w, h, x, y);
			if (b[y][x] && (n == 2 || n == 3))
				new[y][x] = 1;
			else if (!b[y][x] && n == 3)
				new[y][x] = 1;
			else
				new[y][x] = 0;
		}
	}
	free_board(b, h);
	return (new);
}

int	count_neighbors(int **b, int w, int h, int x, int y) {
	int count = 0;
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			if ((i || j) && y + i >= 0 && y + i < h && x + j >= 0 && x + j < w)
				if (b[y + i][x + j])
					count++;
		}
	}
	return (count);
}

// main
int main(int ac, char **av) {
	if (ac != 4)
		return 1;

	int	width = atoi(av[1]);
	int	height = atoi(av[2]);
	int	iterations = atoi(av[3]);
	if (width <= 0 || height <= 0 || iterations < 0)
		return 1;

	int	**board = create_board(width, height);
	read_input(board, width, height);
	for (int i = 0; i < iterations; i++) {
		board = update_board(board, width, height);
	}
	print_board(board, width, height);
	free_board(board, height);
	return 0;
}
