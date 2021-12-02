#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef struct s_bg
{
	int	width;
	int	height;
	char	bgchar;
	FILE	*file;
	char	**matrix;
}	t_bg;
typedef struct s_circl
{
	char	type;
	float	x;
	float	y;
	float	rad;
	char	fillchar;
}	t_circl;

int	ft_strlen(char *str)
{
	int	i = 0;

	if (!str)
		return (i);
	while (str[i])
		i++;
	return (i);
}

void	write_str_nl(char *str)
{
	write(1, str, ft_strlen(str));
	write(1, "\n", 1);
}

void	clear_data(t_bg *bg)
{
	int	i = -1;

	if (bg->file)
		fclose(bg->file);
	if (bg->matrix)
	{
		while (++i < bg->height)
			free(bg->matrix[i]);
		free(bg->matrix);
	}
}

int	my_perror(t_bg *bg, char *errmess)
{
	write_str_nl(errmess);
	clear_data(bg);
	return (1);
}

int	parse_bg(t_bg *bg)
{
	int		scan_ret;
	int		i = -1;

	if ((scan_ret = fscanf(bg->file, "%d %d %c\n", &bg->width, &bg->height, &bg->bgchar)) != 3)
		return (1);
	if (bg->width <= 0 || bg->width > 300 || bg->height <= 0 || bg->height > 300
		|| bg->bgchar < 32 || bg->bgchar > 126)
		return (1);
	if (!(bg->matrix = (char **)malloc(bg->height * sizeof(char *))))
		return (1);
	while (++i < bg->height)
	{
		if (!(bg->matrix[i] = (char *)calloc(bg->width + 1, sizeof(char))))
			return (1);
		memset(bg->matrix[i], bg->bgchar, bg->width);
	}
	return (0);
}

float	dist_to_centre(t_circl *c, float x, float y)
{
	float	dxsq = powf(c->x - x, 2);
	float	dysq = powf(c->y - y, 2);

	return (sqrtf(dxsq + dysq));
}

int	is_in_circle(t_circl *c, int x, int y)
{
	float dist = dist_to_centre(c, (float)x, (float)y);
	if (dist <= c->rad)
	{
		if (dist <= c->rad - 1.00000000)
			return (1); //inside
		return (2); // border
	}
	return (0);
}

void	draw_circle(t_bg *bg, t_circl *c)
{
	int	x = -1;
	int y = -1;
	int	part_of;

	while (++y < bg->height)
	{
		x = -1;
		while (++x < bg->width)
		{
			part_of = is_in_circle(c, x, y);
			if ((c->type == 'c' && part_of == 2)
				|| (c->type == 'C' && part_of))
			{
				bg->matrix[y][x] = c->fillchar;
			}
		}
	}
}

int	parse_draw_circles(t_bg *bg)
{
	int	scan_ret;
	t_circl	c;

	while ((scan_ret = fscanf(bg->file, "%c %f %f %f %c\n", &c.type, &c.x, &c.y, &c.rad, &c.fillchar)) != EOF)
	{
		if (scan_ret == EOF)
			break ;
		if (scan_ret == 1 && c.type == '\n')
			continue ;
		if (scan_ret != 5 || (c.type != 'c' && c.type != 'C') || c.rad <= 0.00000000
			|| c.fillchar < 32 || c.fillchar > 126)
			return (1);
		if (scan_ret == 5)
		{
			draw_circle(bg, &c);
		}
	}
	return (0);
}

void	print_result(t_bg *bg)
{
	int	i = -1;

	while (++i < bg->height)
		write_str_nl(bg->matrix[i]);
}

int	main(int argc, char **argv)
{
	t_bg	bg;

	memset(&bg, 0, sizeof(t_bg));
	if (argc != 2)
		return(my_perror(&bg, "Error: argument"));
	bg.file = fopen(argv[1], "r");
	if (!bg.file)
		return (my_perror(&bg, "Error: Operation file corrupted"));
	if (parse_bg(&bg))
		return (my_perror(&bg, "Error: Operation file corrupted"));
	if (parse_draw_circles(&bg))
		return (my_perror(&bg, "Error: Operation file corrupted"));
	print_result(&bg);
	clear_data(&bg);
}
