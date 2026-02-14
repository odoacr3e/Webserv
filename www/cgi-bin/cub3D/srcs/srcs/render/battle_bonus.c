/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   battle_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alerusso <alessandro.russo.frc@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 14:29:20 by alerusso          #+#    #+#             */
/*   Updated: 2026/02/09 22:34:25 by alerusso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cub3D_bonus.h"

static void	battle_result(t_data *data);
static void	put_textures(t_data *data);
static void	put_result_screen(t_data *data);

static void	main_battle(t_data *data);

void	battle(t_data *data)
{
	int	size[2];
	int	pos[2];

	fill_array(WSCREEN, HSCREEN, size);
	fill_array(0, 0, pos);
	if (DEBUG == false)
		put_image_to_image(data, BATTLE_FRAME, pos, size);
	else
		backgrounder(data);
	main_battle(data);
	//mlx_put_image_to_window(data->mlx, data->win, data->txtr[SCREEN].ptr, 0, 0);
}

static void	put_textures(t_data *data)
{
	int	size[2];
	int	pos[2];
	int	philip;
	int	barbarian;

	philip = PHILIP_CHILL + data->button;
	barbarian = BARBARIAN_CHILL + data->enemy_choice;
	fill_array(300, 450, size);
	fill_array(300, 550, pos);
	put_image_to_image(data, philip, pos, size);
	fill_array(450, 450, size);
	fill_array(1350, 550, pos);
	put_image_to_image(data, barbarian, pos, size);
	fill_array(100, 100, size);
	fill_array(1060, 780, pos);
	put_image_to_image(data, SCISSORS, pos, size);
	fill_array(910, 780, pos);
	put_image_to_image(data, PAPER, pos, size);
	fill_array(760, 780, pos);
	put_image_to_image(data, ROCK, pos, size);
}

static void	main_battle(t_data *data)
{
	if (!data->button || aspettanding(10, 0) == false)
		return (put_textures(data));
	if (!data->result)
		battle_result(data);
	if (aspettanding(30, 1) == false)
		return (put_textures(data));
	put_result_screen(data);
	if (aspettanding(30, 2) == false)
		return ;
	if (data->result == RESULT_WIN)
	{
		data->battle = false;
		data->map[data->foe->map[Y]][data->foe->map[X]] = '0';
		data->foe = NULL;
	}
	if (data->result == RESULT_LOSS)
		ft_cross_close(data);
	data->enemy_choice = 0;
	data->button = 0;
	data->result = 0;
	aspettanding(-1, -1);
}

static void	battle_result(t_data *data)
{
	int	barbarian;

	if (!data->enemy_choice)
		data->enemy_choice = get_rand(CHOICES) + 1;
	barbarian = data->enemy_choice;
	if (data->button == barbarian)
	{
		data->result = RESULT_DRAW;
	}
	else if ((data->button == CHOICE_ROCK && barbarian == CHOICE_PAPER) || \
(data->button == CHOICE_PAPER && barbarian == CHOICE_SCISSOR) || \
(data->button == CHOICE_SCISSOR && barbarian == CHOICE_ROCK))
	{
		data->result = RESULT_LOSS;
	}
	else
		data->result = RESULT_WIN;
}

static void	put_result_screen(t_data *data)
{
	int	size[2];
	int	pos[2];
	int	which;
	int	la_madonna;

	la_madonna = 0;
	if (data->result == RESULT_DRAW)
		which = DRAW_MSG;
	else if (data->result == RESULT_WIN)
		which = WIN_MSG;
	else if (data->result == RESULT_LOSS)
	{
		if (aspettanding(30, 0) == true)
			data->menu = -1;
		which = LOSS_MSG;
	}
	else
		which = MENU_FRAME;
	la_madonna = (data->txtr[which].size[X] / (data->txtr[which].bpp / 8)) / 2;
	fill_array(((WSCREEN / 2) - la_madonna), HSCREEN / 2, pos);
	size[X] = data->txtr[which].size[X] / (data->txtr[which].bpp / 8);
	size[Y] = data->txtr[which].size[Y];
	put_image_to_image(data, which, pos, size);
}
