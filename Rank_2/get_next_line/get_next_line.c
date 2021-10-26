#include "get_next_line.h"

int	ft_strchr(char *str, char c)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
	{
		if (str[i] == c)
			return (1);
		i++;
	}
	return (0);
}

int	ft_strlen(char *str)
{
	int	i;

	if (!str)
		return (0);
	i = 0;
	while (str[i])
		i++;
	return(i);
}

void	ft_putstr(char *str)
{
	write(1, str, ft_strlen(str));
}

char	*cut_line(char *keep)
{
	char	*new;
	int		i;
	int		j;

	i = 0;
	while (keep[i] && keep[i] != '\n')
		i++;
	if (!(new = malloc(ft_strlen(keep) - i + 1)))
		return (NULL);
	if (keep[i] == '\n')
	{
		i++;
		j = 0;
		while (keep[i])
			new[j++] = keep[i++];
		new[j] = '\0';
	}
	else	
		new[0] = '\0';
	if (keep)
		free(keep);
	return (new);
}

char	*join(char *keep, char *buf)
{
	char 	*new;
	int		len;
	int		i;

	len = 0;
	if (keep)
		len += ft_strlen(keep);
	len += ft_strlen(buf);
	if (!(new = malloc(sizeof(char) * (len + 1))))
		return (NULL);
	len = 0;
	if (keep)
	{
		i = 0;
		while (keep[i])
			new[len++] = keep[i++];
		free(keep);
	}
	i = 0;
	while (buf[i])
		new[len++] = buf[i++];
	new[len] = '\0';
	return (new);
}

char	*push_line(char *keep)
{
	char	*new;
	int		i;
	int		j;

	i = 0;
	while (keep[i] && keep[i] != '\n')
		i++;
	if (!(new = malloc(i + 1 + ft_strchr(keep, '\n'))))
		return (NULL);
	j = 0;
	while (j <= i)
	{
		new[j] = keep[j];
		j++;
	}
	if (keep[i] == '\n')
		new[j] = '\0';
	return (new);
}

char	*get_next_line(int fd)
{
	static char	*keep;
	char		*line;
	char		*buf;
	int			r;

	r = 1;
	if (!(buf = malloc(sizeof(char) * (BUFFER_SIZE + 1))))
		return (NULL);
	while (r > 0 && !ft_strchr(keep, '\n'))
	{
		r = read(fd, buf, BUFFER_SIZE);
		if (r == -1)
			return (NULL);
		buf[r] ='\0';
		keep = join(keep, buf);
		if (!keep)
			return (NULL);
	}
	line = push_line(keep);
	if (!line)
		return (NULL);
	keep = cut_line(keep);
	if (!keep)
		return (NULL);
	free(buf);
	if (r == 0 && keep[0] == 0)
		return (NULL);
	return (line);
}

