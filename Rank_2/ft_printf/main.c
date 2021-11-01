#include <stdio.h>

int	ft_printf(const char *sr, ...);

int		main()
{
	printf("\n___TEST___\n");
	printf("Copy - %d\n", ft_printf("|%20.s|\n", NULL));
	printf("Original - %d\n", printf("|%20.s|\n", NULL));
	return 0;
}
