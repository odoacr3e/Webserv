/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alerusso <alessandro.russo.frc@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 11:39:28 by lparolis          #+#    #+#             */
/*   Updated: 2026/02/12 16:20:52 by alerusso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3D_bonus.h"

/*
	FUNCTION NAME			DESCRIPTION
_________________________________________________________
	Parsing:				get and validate map;
	lets_start_the_party:	initialize mlx/texture data;
	mlx_hooks:				initialize keyboard;
	map_start:				prints map for the first time;
	game_loop:				the game main loop;
	spread_democracy:		free everything and exit.
*/
int	main(int argc, char *argv[])
{
	t_data	data;

	data = (t_data){0};
	set_txtr_null(&data);
	parsing(&data, argc, argv);
	lets_start_the_party(&data);
	gettimeofday(&data.start, NULL);
	webserv_porting(&data);
	spread_democracy(&data);
	return (0);
}
