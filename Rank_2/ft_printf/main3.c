#include <stdio.h>
int		ft_printf(const char *s, ...);

int main(void) {
	char *s = 0;
  printf("f:= %d\n", ft_printf("Hello World (%2.4d)",12));
  printf("p:= %d\n", printf("Hello World (%2.4d)", 12));
  printf("f:= %d\n", ft_printf("%s", s));
  printf("p:= %d\n", printf("%s", s));
  return 0;
}
