#include "minishell.h"

int	is_excutable(struct stat fst)
{
	if (fst.st_mode & S_IXUSR)
	{
		if (fst.st_mode & S_IFDIR)
			return (FOLDER);
		return (0);
	}
	return (NEX);
}
