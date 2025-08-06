#include "life.h"

// utils
char	**create_board(int w, int h) {
	char	**b = calloc(h, sizeof(char *));
	if (!b)
		return (NULL);
	
	for (int i = 0; i < h; i++) {
		b[i] = calloc(w, sizeof(char));
		if (!b[i])
			return (NULL);
	}
	return (b);
}

void	free_board(char **b, int h) {
	for (int i = 0; i < h; i++) {
		if (b[i])
			free(b[i]);
	}
	if (b)
		free(b);
}

void	print_board(char **b, int w, int h) {
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
void	read_input(char **b, int w, int h) {
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

char	**update_board(char **b, int w, int h) {
	int		n;
	char	**new = create_board(w, h);

	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			n = count_neighbors(b, j, i, w, h);
			if (b[i][j] && (n == 2 || n == 3))
				new[i][j] = 1;
			else if (!b[i][j] && n == 3)
				new[i][j] = 1;
			else
				new[i][j] = 0;
		}
	}
	free_board(b, h);
	return (new);
}

int	count_neighbors(char **b, int x, int y, int w, int h) {
	int count = 0;
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			if (i || j)
				if (y + i >= 0 && y + i < h && x + j >= 0 && x + j < w)
					if (b[y + i][x + j])
						count++;
		}
	}

	return (count);
}

// main
int main(int ac, char **av) {
	int		width, height, iterations;
	char	**board;

	if (ac != 4)
		return 1;

	width = atoi(av[1]);
	height = atoi(av[2]);
	iterations = atoi(av[3]);
	if (width <= 0 || height <= 0 || iterations < 0)
		return 1;

	board = create_board(width, height);
	if (!board)
		return 1;

	read_input(board, width, height);
	for (int i = 0; i < iterations; i++) {
		board = update_board(board, width, height);
	}

	print_board(board, width, height);
	free_board(board, height);

	return 0;
}

// cc -Wall -Werror -Wextra life.c
