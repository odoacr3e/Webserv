/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alerusso <alessandro.russo.frc@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 17:12:16 by lparolis          #+#    #+#             */
/*   Updated: 2026/02/14 03:32:47 by alerusso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cub3D_bonus.h"

//static int	check_file_extension(char *file);
static void	parse_cub(t_data *data, int fd);

/* REVIEW
1)	Controllo numero di argomenti
2)	Controllo estensione del file passato come argomento
3)	Controllo che l'argomento esista
4)	Assegnazione dei vari type identifiers e della mappa (parse_cub)

	Type identifiers = SO, NO, WE, EA, F, C.
*/
void	parsing(t_data *data, int argc, char **argv)
{
	int	fd;

	(void)argc, (void)argv;
	fd = -1;
	fd = open("www/cgi-bin/cub3D/debug.cub", O_RDONLY);
	if (fd < 0)
		error(data, E_OPEN, argv[1]);
	parse_cub(data, fd);
}

/*	strrchr moves file ptr to the last '.' it finds.
static int	check_file_extension(char *file)
{
	file = ft_strrchr(file, '.');
	if (file)
		if (ft_strncmp(file, ".cub", 4) == 0)
			return (0);
	return (1);
}*/

/*
//REVIEW parse_cub
	
	FUNCTION		DESCRIPTION
________________________________________________________________
	get_type		finds in the file the names of the textures.
	check_textures	checks textures are present and colors are valid.
	get_map			alloc and get map data.
	parse_map		checks player, walls and if all map is accessible.
*/
static void	parse_cub(t_data *data, int fd)
{
	check_textures(data, fd);
	get_map(data, fd);
	parse_map(data);
	gnl_statik(fd, true);
}
