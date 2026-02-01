#include "../all.h"

static void	print_field(char **field);
static void	hypertext(bool href_flag, char *val);

int	main(int ac, char **av)
{
	char	***data;

	if (ac == 1)
		return (ft_printf("Please give a valid pokemon name\n"));
	STR(pokemon, av[1]);
	pokemon.m->str_upper(&pokemon);
	if (daft_init("www/cgi-bin/pokedex/media", "SETTINGS.md") != 0)
		daft_init("media", "SETTINGS.md");
	daft_swap(POKEDEX);
	data = daft_get(pokemon.buff);
	if (!data)
	{
		ft_printf("pokemon %s not found.\n", pokemon.buff);
		return (str_terminate(), daft_quit(), 1);
	}
	for (int i = 0; data[i]; i++)
		print_field(data[i]);
	return (str_terminate(), daft_quit(), 0);
}

#define KEY_MOVES	 	"Moves"
#define KEY_MOVES_TUTOR "TutorMoves"
#define KEY_MOVES_EGG	"EggMoves"
#define KEY_NAME		"Name"
#define KEY_EVOLUTION	"Evolutions"
#define HREF_KEYS 	KEY_MOVES, KEY_MOVES_TUTOR, KEY_MOVES_EGG,\
					KEY_NAME, KEY_EVOLUTION, NULL

static void	print_field(char **field)
{
	static const char	*href_keys[] = {HREF_KEYS};
	char				*key;
	bool				href_flag;

	key = field[0];
	href_flag = false;
	ft_printf("|%s:", key);
	for (int i = 0; href_keys[i]; i++)
	{
		if (ft_strncmp(key, href_keys[i], INT_MAX) == 0)
			href_flag = true;
	}
	for (int i = 1; field[i]; i++)
		ft_printf("?%p-%d-%p?%s,", hypertext, href_flag, field[i], field[i]);
}

static void	hypertext(bool href_flag, char *val)
{
	if (href_flag == false)
		return ;
	for (int i = 0; val[i]; i++)
	{
		if (val[i] < 'A' || val[i] > 'Z')
			return ;
	}
	ft_printf("@");
}
