#include "pokedex.h"

static void	print_field(char **field, t_str *buff);
static void	hypertext(int href_flag, char *val, t_str *buff);
static void	pkmn_id(char ***data, t_str *buff);

int	print_cgi(char *output, int output_len)
{
	char	*str;
	int		len;
	int		padding_zero;

	write(1, "OK|", 3);
	str = ft_itoa(output_len);
	if (!str)
		return (1);
	len = ft_strlen(str);
	padding_zero = 10 - len;
	if (padding_zero > 0)
		write(1, "00000000000", padding_zero);
	write(1, str, len);
	write(1, "|", 1);
	write(1, output, output_len);
	free(str);
	return (0);
}

int	error_cgi(char *error)
{
	write(1, "KO|00000000-1|", 14);
	write(1, error, ft_strlen(error));
	return (0);
}

int	main(int ac, char **av)
{
	char	***data;

	if (ac == 1)
		return (error_cgi("Please give a valid pokemon name\n"));
	STR(pokemon, av[1]);
	STR(buff, "");
	pokemon.m->str_upper(&pokemon);
	if (daft_init("www/cgi-bin/pokedex/media", "SETTINGS.md") != 0)
		daft_init("media", "SETTINGS.md");
	daft_swap(POKEDEX);
	data = daft_get(pokemon.buff);
	if (!data)
	{
		str_printf(&buff, "pokemon %s not found.\n", pokemon.buff);
		FREE(pokemon.m);
		error_cgi(buff.buff);
		return (str_terminate(), daft_quit(), 1);
	}
	pkmn_id(data, &buff);
	for (int i = 0; data[i]; i++)
		print_field(data[i], &buff);
	print_cgi(buff.buff, buff.len);
	FREE(pokemon.m);
	return (str_terminate(), daft_quit(), 0);
}

static void	print_field(char **field, t_str *buff)
{
	static const char	*href_keys[] = {HREF_KEYS};
	char				*key;
	int					href_flag;

	key = field[0];
	href_flag = NO_HREF;
	str_printf(buff, "|%s:", key);
	for (int i = 0; href_keys[i]; i++)
	{
		if (ft_strncmp(key, href_keys[i], INT_MAX) == 0)
			href_flag = i;
	}
	for (int i = 1; field[i]; i++)
	{
		hypertext(href_flag, field[i], buff);
		str_printf(buff, "%s,", field[i]);
	}
}

static void	hypertext(int href_flag, char *val, t_str *buff)
{
	if (href_flag == NO_HREF)
		return ;
	for (int i = 0; val[i]; i++)
	{
		if (val[i] < 'A' || val[i] > 'Z')
			return ;
	}
	if (href_flag <= RANGE_MOVE)
		href_flag = ID_MOVE;
	else if (href_flag <= RANGE_PKMN)
		href_flag = ID_PKMN;
	else
		return (str_printf(buff, "pokemon.cgi: enum error\n"), (void)0);
	str_printf(buff, "@%d", href_flag);
}

static void	pkmn_id(char ***data, t_str *buff)
{
	int	i;

	for (i = 0; data[i]; i++)
	{
		if (ft_strncmp("ID", data[i][0], INT_MAX) == 0)
			break ;
	}
	if (!data[i])
		return (str_printf(buff, "|not_found"), (void)0);
	str_printf(buff, "{%d}%s", ID_PKMN, data[i][1]);
}
