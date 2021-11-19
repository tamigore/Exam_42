#include "mini_paint.h"

int		ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

void	ft_putstr(char *str, int len)
{
	int	i;

	i = 0;
	while (str[i] && i < len)
		write(1, &str[i++], 1);
}

int		dist(float x, float y, float cx, float cy, char type)
{
	float	dist;

	dist = sqrtf((x - cx) * (x - cx) + (y - cy) * (y - cy));
	if (dist <= 1)
	{
		if (type == 'C')
			return (1);
		else
		{
			if (dist <= 1 && dist >= 0.9)
				return (1);
		}
	}
	return (0);
}

void	put_obj(t_env *env)
{
	int	x;
	int	y;

	y = 0;
	while (y < env->draw.height)
	{
		x = 0;
		while (x < env->draw.width)
		{
			if (dist(x, y, env->obj.x, env->obj.y, env->obj.type))
				env->draw.matrice[x][y] = env->obj.color;
			x++;
		}
	}
}

void	print_tab(t_env *env)
{
	int	i;

	i = 0;
	while (env->draw.matrice[i])
	{
		ft_putstr(env->draw.matrice[i++], env->draw.width);
		ft_putstr("\n", 1);
	}
}

int	pars(FILE *fd, t_env *env)
{
	int		ret;
	char	*str;
	int		i;
	int		j;

	str = NULL;
	ret = fscanf(fd, "%[^\n]", &str);
	printf("str:%s\n", str);
	if (ret != 3 || env->draw.width < 1 || env->draw.width > 300 || env->draw.height < 1 || env->draw.height > 300)
	{	
		ft_putstr("bug fscanf\n", ft_strlen("bug fscanf\n"));
		return (1);
	}
	env->draw.matrice = malloc(sizeof(char *) * (env->draw.height + 1));
	if (!env->draw.matrice)
	{
		ft_putstr("bug matrice\n", ft_strlen("bug matrice\n"));
		return (1);
	}
	i = 0;
	while (i < env->draw.height)
	{
		env->draw.matrice[i] = malloc(sizeof(char) * (env->draw.width + 1));
		if (!env->draw.matrice[i])
		{
			ft_putstr("bug matrice[i]\n", ft_strlen("bug matrice[i]\n"));
			return (1);
		}
		i++;
	}
	env->draw.matrice[i] = NULL;
	i = 0;
	ft_putstr("start\n", ft_strlen("start\n"));
	while (i < env->draw.height)
	{
		j = 0;
		while (j < env->draw.width)
			env->draw.matrice[i][j++] = env->draw.c;
		env->draw.matrice[i][j] = '\0';
		i++;
	}
	ret = 5;
	while ((ret = fscanf(fd, "%c %f %f %f %c", &env->obj.type, &env->obj.x, &env->obj.y, &env->obj.radius, &env->obj.color)) != EOF)
	{
		if (ret != 5)
		{
			printf("ret = %d: bug\n", ret);
			printf("type = %c\n", env->obj.type);
			return (1);
		}
		if (env->obj.radius <= 0 || (env->obj.type != 'C' && env->obj.type != 'c'))
		{
			ft_putstr("bug obj\n", ft_strlen("bug obj\n"));
			return (1);
		}
		put_obj(env);
	}
	print_tab(env);
	return (0);
}

int main(int ac, char **av)
{
	FILE	*fd;
	t_env	env;
	int		err;

	if (ac != 2)
	{
		ft_putstr("Error: argument", ft_strlen("Error: argument"));
		return (1);
	}
	fd = fopen(av[1], "r");
	if (!fd)
	{
		ft_putstr("Error: Operation file corrupted", ft_strlen("Error: Operation file corrupted"));
		return (1);
	}
	ft_putstr("in\n", ft_strlen("in\n"));
	err = pars(fd, &env);
	if (err == 1)
	{
		ft_putstr("Error: err = 1\n", ft_strlen("Error: err = 1\n"));
		return (1);
	}
	return (0);
}