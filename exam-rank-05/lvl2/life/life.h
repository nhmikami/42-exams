#ifndef LIFE_H
#define LIFE_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

char	**create_board(int w, int h);
void	free_board(char **b, int h);
void	print_board(char **b, int w, int h);

void	read_input(char **b, int w, int h);
char	**update_board(char **b, int w, int h);
int     count_neighbors(char **b, int x, int y, int w, int h);

#endif