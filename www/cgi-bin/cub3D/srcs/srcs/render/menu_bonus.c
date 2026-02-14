/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   menu_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alerusso <alessandro.russo.frc@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 10:53:43 by alerusso          #+#    #+#             */
/*   Updated: 2026/02/12 16:29:53 by alerusso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cub3D_bonus.h"

// static void	render_button(t_data *data);
static void	toggle_button(t_data *data, int button, bool on);
static int	the_frame(int *time, int nframes);

void	main_menu(t_data *data)
{
	int	pos[2];
	int	size[2];

	if (data->menu < 0)
	{
		hai_guardato(data);
		return ;
	}
	fill_array(0, 0, pos);
	fill_array(WSCREEN, HSCREEN, size);
	if (DEBUG == false)
		put_image_to_image(data, MENU_FRAME, pos, size);
	fill_array(700, 355, pos);
	fill_array(500, 150, size);
	toggle_button(data, PLAY_BUTTON, data->player.input & UP);
	put_image_to_image(data, PLAY_BUTTON, pos, size);
	fill_array(700, 600, pos);
	toggle_button(data, EXIT_BUTTON, data->player.input & DOWN);
	put_image_to_image(data, EXIT_BUTTON, pos, size);
	//mlx_put_image_to_window(data->mlx, data->win, data->txtr[SCREEN].ptr, 0, 0);
}

static void	toggle_button(t_data *data, int button, bool on)
{
	if (on)
	{
		data->txtr[button].filters |= FILTER_BUTTON_PRESSED;
		if (button == PLAY_BUTTON)
			data->button |= PLAY;
		else
			data->button |= EXIT;
	}
	else
	{
		bitwise_xor(&data->txtr[button].filters, (char)FILTER_BUTTON_PRESSED);
		if (button == PLAY_BUTTON)
			bitwise_xor(&data->button, PLAY);
		else
			bitwise_xor(&data->button, EXIT);
	}
}

void	hai_guardato(t_data *data)
{
	int	death_frame;
	int	pos[2];
	int	size[2];

	if (data->menu == -1)
	{
		clear_window(data);
		ft_memcpy(&data->events[3], SFX_HAI_GUARDATO, ft_strlen(SFX_HAI_GUARDATO));
		data->menu = -2;
	}
	fill_array(0, 0, pos);
	fill_array(1920, 1080, size);
	death_frame = the_frame((int [7]){10, 10, 10, 16, 24, 50, 65}, 7);
	if (death_frame == -1)
		ft_cross_close(data);
	if (death_frame <= DEATH7)
		put_image_to_image(data, death_frame, pos, size);
	else
		clear_window(data);
	//mlx_put_image_to_window(data->mlx, data->win, data->txtr[SCREEN].ptr, 0, 0);
}

static int	the_frame(int *time, int nframes)
{
	int	i;

	i = 0;
	while (i < nframes)
	{
		if (aspettanding(time[i], DEATH1 + i) == false)
			return (DEATH1 + i);
		i += 1;
	}
	if (aspettanding(1, 0) == true)
		return (-1);
	return (DEATH1 + i);
}
