#include "../all.h"

static char	**get_info();

int	main(int ac, char **av)
{
	char	***data;
	char	**info;

	if (ac == 1)
		return (ft_printf("Please give a valid move name\n"));
	info = get_info();
	STR(move, av[1]);
	move.m->str_lower(&move);
	if (daft_init("www/cgi-bin/pokedex/media", "SETTINGS.md") != 0)
		daft_init("media", "SETTINGS.md");
	daft_swap(MOVES);
	data = daft_get(move.buff);
	if (!data || !info)
	{
		free_matrix(info);
		ft_printf("move %s not found.\n", move.buff);
		return (str_terminate(), daft_quit(), 1);
	}
	for (int i = 1; data[i]; i++)
	{
		ft_printf("|%s:%s", info[i], data[i]);
	}
	return (str_terminate(), free_matrix(info), daft_quit(), 0);
}

static char	**get_info()
{
	t_fd	fd;
	char	*line;
	char	**info;

	fd = openfd("www/cgi-bin/pokedex/media/data/metadata_pokemon_moves.csv", "r");
	if (!fd.p)
		fd = openfd("media/data/metadata_pokemon_moves.csv", "r");
	if (!fd.p)
		return (err_printf("moves.cgi error: missing files\n"), NULL);
	line = gnl();
	closefd(fd);
	if (!line)
		return (err_printf("moves.cgi error: empty data\n"), NULL);
	info = ft_split(line, ',');
	FREE(line);
	return (info);
}
