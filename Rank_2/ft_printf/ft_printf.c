#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>

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
	return (write(1, str, ft_strlen(str)));
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
	if (size <= 0)
		return (0);
	return (write(1, &c, size));
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
	{
		if (!(str = get_num(nb, check_long(nb < 0 ? (unsigned int)-nb : (unsigned int)nb) + (nb < 0 ? 1 : 0))))
			return (-1);
	}
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
	{
		if (!(str = get_long(nb, check_long(nb))))
			return (-1);
	}
	count += put_opt_hexa(opt, str);
	count += ft_putstr(str);
	return (count);
}

int	put_str(t_lst opt, va_list ap)
{
	int count;
	char	*str;
	int	i;

	i = 0;
	count = 0;
	str = va_arg(ap, char *);
	if (!str && opt.width == 0)
		str = "(null)";
	else
		str = "";
	if (opt.width)
		count += put_char(opt.width, ' ');
	if (opt.preci > 0)
	{
		while (i < opt.preci)
			count += put_char(1, str[i++]);
	}
	else
		count += ft_putstr(str);
	return (count);
}

int	pars_id(char *str, t_lst opt, va_list ap)
{
	int	count;
	int tmp;
	int	i;

	count = 0;
	i = 0;
	if (str[i] == 'd')
	{
		if ((tmp = put_int(opt, ap)) == -1)
			return (-1);
		count += tmp;
	}
	else if (str[i] == 'x')
	{
		if ((tmp = put_hexa(opt, ap)) == -1)
			return (-1);
		count += tmp;
	}
	else if (str[i] == 's')
	{
		if ((tmp = put_str(opt, ap)) == -1)
			return (-1);
		count += tmp;
	}
	return (count);
}

int	ft_printf(const char *str, ...)
{
	va_list			ap;
	unsigned int	count;
	int				i = 0;
	int				tmp;
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
			if ((tmp = pars_id((char *)&str[i], opt, ap)) == -1)
				return (count);
			count += tmp;
		}
		else
			count += write(1, &str[i], 1);
		i++;
	}
	return (count);
}