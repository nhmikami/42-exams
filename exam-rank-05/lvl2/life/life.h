#ifndef LIFE_H
#define LIFE_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int		**create_board(int w, int h);
void	free_board(int **b, int h);
void	print_board(int **b, int w, int h);

void	read_input(int **b, int w, int h);
int		**update_board(int **b, int w, int h);
int		count_neighbors(int **b, int w, int h, int x, int y);

#endif