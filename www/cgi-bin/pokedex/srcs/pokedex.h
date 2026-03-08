#ifndef POKEDEX_H
# define POKEDEX_H
# include "../all.h"

#define KEY_MOVES	 	"Moves"
#define KEY_MOVES_TUTOR "TutorMoves"
#define KEY_MOVES_EGG	"EggMoves"
#define KEY_NAME		"Name"
#define KEY_EVOLUTION	"Evolutions"
#define HREF_KEYS 	KEY_MOVES, KEY_MOVES_TUTOR, KEY_MOVES_EGG,\
					KEY_NAME, KEY_EVOLUTION, NULL

typedef enum e_href_type
{
	NO_HREF = -1,
	RANGE_MOVE = 2,
	RANGE_PKMN = 4,
	ID_MOVE = 0,
	ID_PKMN = 1,
}			t_href_type;

#endif