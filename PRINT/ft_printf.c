#include <unistd.h>
#include <stdarg.h>

void ft_printf(int count, ...)
{
	va_list list;
	int i;
	char *str;
	int j;

	i = -1;
	va_start(list, count);
	while (++i < count)
	{
		j = -1;
		str = va_arg(list, char *);
		while (str[++j])
			write(1, &(str[j]), 1);
	}
	va_end(list);
}
