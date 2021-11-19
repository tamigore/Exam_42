#ifndef MINI_PAINT_H
# define MINI_PAINT_H

# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <unistd.h>

typedef struct s_drawing
{
	int		width;
	int		height;
	char	c;
	char	**matrice;
}	t_draw;

typedef struct s_circle
{
	char type;
	float x;
	float y;
	float radius;
	char color;
}	t_obj;

typedef struct s_env
{
    struct s_circle     obj;
    struct s_drawing    draw;
}	t_env;

#endif