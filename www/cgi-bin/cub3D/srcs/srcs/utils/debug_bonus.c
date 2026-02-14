/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alerusso <alessandro.russo.frc@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 09:13:59 by alerusso          #+#    #+#             */
/*   Updated: 2026/02/09 22:35:21 by alerusso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cub3D_bonus.h"

bool	value_changed(void *value, size_t type)
{
	static void	*old_value;

	if (!old_value)
		old_value = value;
	if (type == sizeof(int8_t) && \
*(int8_t *)old_value != *(int8_t *)value)
	{
		*(int8_t *)old_value = *(int8_t *)value;
		return (true);
	}
	else if (type == sizeof(int16_t) && \
*(int16_t *)old_value != *(int16_t *)value)
	{
		*(int16_t *)old_value = *(int16_t *)value;
		return (true);
	}
	else if (type == sizeof(int32_t) && \
*(int32_t *)old_value != *(int32_t *)value)
	{
		*(int32_t *)old_value = *(int32_t *)value;
		return (true);
	}
	return (false);
}

void	render_sync(t_data *data)
{(void)data;
	//mlx_put_image_to_window(data->mlx, data->win, data->txtr[SCREEN].ptr, 0, 0);
	//mlx_do_sync(data->mlx);
}
