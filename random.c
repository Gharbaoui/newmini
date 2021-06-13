#include "minishell.h"

int	help_ft_atoi(const char *str, int np)
{
	long	result;
	long	max;

	result = 0;
	max = 9223372036854775807;
	while (ft_isdigit(*str))
	{
		if (result <= (max - (*str - '0')) / 10)
			result = (result * 10) + *str - '0';
		else
		{
			if (np == -1)
				result = 0;
			else
				result = -1;
			break ;
		}
		str++;
	}
	return (result * np);
}

int	ft_atoi(char *str)
{
	int		np;

	np = 1;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str++ == '-')
			np = -1;
		else
			np = 1;
	}
	return (help_ft_atoi(str, np));
}

char	*ft_itoa(int value)
{
	char	*buffer;
	int		i;
	int		r;
	char	*tmp;

	i = 0;
	buffer = malloc(sizeof(char) * 4);
	while (value)
	{
		r = value % 10;
		if (r >= 10)
			buffer[i++] = 65 + (r - 10);
		else
			buffer[i++] = 48 + r;
		value /= 10;
	}
	if (i == 0)
		buffer[i++] = '0';
	buffer[i] = '\0';
	tmp = ft_reverse(buffer);
	free(buffer);
	return (tmp);
}

char	*ft_reverse(char *line)
{
	int		i;
	int		j;
	char	*tmp;

	j = ft_strlen(line);
	tmp = malloc(j + 1);
	i = -1;
	while (line[++i])
		tmp[i] = line[--j];
	tmp[i] = 0;
	return (tmp);
}
