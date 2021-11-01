#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
typedef struct	s_lst
{
	int	width;
	int	point;
	int	preci;
}	t_lst;

t_lst	init(t_lst opt)
{
	opt.width = 0;
	opt.point = 0;
	opt.preci = 0;
	return (opt);
}

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

int	ft_putstr(char *str)
{
	int	i;
	int count;

	i = 0;
	count = 0;
	while (str[i])
	{
		count += write(1, &str[i], 1);
		i++;
	}
	return (count);
}

int	is_id(char c)
{
	if (c == 'd' || c == 'x' || c =='s')
		return (1);
	return (0);
}

int	is_digit(char c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

int	pars_arg(char *str, t_lst *opt)
{
	int	i;

	i = 0;
	while (str[i] && !is_id(str[i]))
	{
		if (opt->point == 0 && is_digit(str[i]))
			opt->width = opt->width * 10 + str[i] - '0';
		else if (opt->point == 0 && str[i] == '.')
			opt->point = 1;
		else if (opt->point == 1 && is_digit(str[i]))
			opt->preci = opt->preci * 10 + str[i] - '0';
		i++;
	}
	return	(i);
}

int	check_long(unsigned int number)
{
	int	len;

	len = 0;
	if (number == 0)
		return (1);
	while (number > 0)
	{
		len++;
		number /= 16;
	}
	return (len);
}

char	*get_num(int number, int size)
{
	unsigned	num;
	char		*str;

	str = malloc(sizeof(char ) * (size + 1));
	str[size] = '\0';
	if (number == 0)
	{
		str[0] = '0';
		return (str);
	}
	if (number < 0)
		num = number * -1;
	else
		num = number;
	while (--size > 0)
	{
		str[size] = (num % 10) + '0';
		num /= 10;
	}
	if (number < 0)
		str[size] = '-';
	else
		str[size] = (num % 10) + '0';
	return (str);
}

char	*get_long(unsigned int number, int size)
{
	char	*str;
	char	*array_hex = "0123456789abcdef";

	str = malloc(sizeof(char) * (size + 1));
	str[size] = '\0';
	if (number == 0)
	{
		str[0] = '0';
		return (str);
	}
	while (--size > 0)
	{
		str[size] = array_hex[number % 16];
		number /= 16;
	}
	str[size] = array_hex[number % 16];
	return (str);
}

int	put_char(int size, char c)
{
	int	i;
	int	count;

	if (size <= 0)
		return (0);
	i = 0;
	count = 0;
	while (i < size)
	{
		i++;
		count += write(1, &c, 1);
	}
	return (count);
}

int	put_opt_int(t_lst opt, char *str)
{
	int	count;
	int	len;
	int	neg;

	count = 0;
	neg = (str[0] == '-' ? 1 : 0);
	len = ft_strlen(str);
	if (opt.width > 0)
		count += put_char(opt.width - (opt.preci > len - neg ? opt.preci + neg : len), ' ');
	if (opt.preci > 0)
		count += put_char((opt.preci > len - neg ? opt.preci - len : 0), '0');
	return (count);
}

int	put_opt_hexa(t_lst opt, char *str)
{
	int	count;
	int	len;

	count = 0;
	len = ft_strlen(str);
	if (opt.width > 0)
		count += put_char(opt.width - (opt.preci > len ? opt.preci : len), ' ');
	if (opt.preci > 0)
		count += put_char((opt.preci > len ? opt.preci - len : 0), '0');
	return (count);
}

int	put_int(t_lst opt, va_list ap)
{
	int	count;
	int	nb;
	char *str;

	count = 0;
	nb = va_arg(ap, int);
	if (nb == 0 && opt.point == 1 && opt.preci == 0)
		str = "";
	else
		str = get_num(nb, check_long(nb < 0 ? (unsigned int)-nb : (unsigned int)nb) + (nb < 0 ? 1 : 0));
	count += put_opt_int(opt, str);
	count += ft_putstr(str);
	if (nb != 0 || opt.point == 0 || opt.preci > 0)
		free(str);
	return (count);
}

int	put_hexa(t_lst opt, va_list ap)
{
	int	count;
	char *str;
	unsigned int	nb;

	count = 0;
	nb = va_arg(ap, unsigned int);
	if (nb == 0 && opt.point == 1 && opt.preci == 0)
		str = "";
	else
		str = get_long(nb, check_long(nb));
	count += put_opt_hexa(opt, str);
	count += ft_putstr(str);
	return (count);
}

char *ft_strdup(char *str)
{
	int	i;
	char *res;

	if (!str)
		return (NULL);
	res = malloc(ft_strlen(str));
	i = 0;
	while (str[i])
	{
		res[i] = str[i];
		i++;
	}
	res[i] = '\0';
	return (res);
}

int	put_str(t_lst opt, va_list ap)
{
	int count;
	char	*str;
	int	i;

	count = 0;
	str = ft_strdup(va_arg(ap, char *));
	if (!str && opt.width == 0)
		str = ft_strdup("(null)");
	else
		str = ft_strdup("");
	if (opt.width > 0)
		count += put_char(opt.width - (opt.preci > 0 ? opt.preci : ft_strlen(str)), ' ');
	i = 0;
	if (opt.preci > 0)
	{
		while (i < opt.preci)
			count += put_char(1, str[i++]);
	}
	else
		count += ft_putstr(str);
	free(str);
	return (count);
}

int	pars_id(char *str, t_lst opt, va_list ap)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	if (str[i] == 'd')
		count += put_int(opt, ap);
	else if (str[i] == 'x')
		count += put_hexa(opt, ap);
	else if (str[i] == 's')
		count += put_str(opt, ap);
	return (count);
}

int	ft_printf(const char *str, ...)
{
	va_list			ap;
	unsigned int	count;
	int				i = 0;
	t_lst			opt;

	va_start(ap, str);
	count = 0;
	while (str[i])
	{
		if (str[i] == '%')
		{
			i++;
			opt = init(opt);
			i += pars_arg((char *)&str[i], &opt);
			count += pars_id((char *)&str[i], opt, ap);
		}
		else
			count += write(1, &str[i], 1);
		i++;
	}
	return (count);
}
