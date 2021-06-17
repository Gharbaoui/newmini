#include "minishell.h"

char	*cutstring(char *str, int start, int last)
{
	char	*tmp;
	int		len;

	len = last - start + 1;
	tmp = malloc(len);
	if (!tmp)
		return (0);
	--start;
	len = -1;
	while (++start < last)
		tmp[++len] = str[start];
	tmp[++len] = 0;
	return (tmp);
}

int	is_digit(char c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

int	is_number(char *num)
{
	if (*num == '-' || *num == '+')
		num++;
	while (*num)
	{
		if (!is_digit(*num))
			return (0);
		num++;
	}
	return (1);
}

void	ft_printf(int count, ...)
{
	va_list	list;
	int		i;
	char	*str;
	int		j;

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

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = -1;
	while (s1[++i])
		if (s1[i] != s2[i])
			break ;
	return (s1[i] - s2[i]);
}
