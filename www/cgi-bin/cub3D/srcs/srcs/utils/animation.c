/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   animation.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alerusso <alerusso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 15:59:16 by alerusso          #+#    #+#             */
/*   Updated: 2025/08/28 17:40:29 by alerusso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cub3D_bonus.h"

void	update_all_animations(t_data *data)
{
	int	i;

	i = -1;
	while (data->doors && data->doors[++i].type != ENTITY_END)
		update_animation(&data->doors[i]);
	i = -1;
	while (data->enemies && data->enemies[++i].type != ENTITY_END)
		update_animation(&data->enemies[i]);
	i = -1;
	while (data->items && data->items[++i].type != ENTITY_END)
		update_animation(&data->items[i]);
}

void	update_animation(t_entity *entity)
{
	entity->f_time = ANIMATION_SPEED / entity->speed;
	if (!entity->frames[entity->f_curr])
	{
		entity->f_curr = 0;
		entity->f_elapsed = 0;
		return ;
	}
	if (entity->f_elapsed >= entity->f_time)
	{
		entity->f_curr += 1;
		entity->f_elapsed = 0;
	}
	if (entity->frames[entity->f_curr] == NULL)
	{
		entity->f_curr = 0;
		entity->f_elapsed = 0;
		return ;
	}
	entity->f_elapsed += 1;
}

void	init_animation(t_data *data, t_entity *entity, int n, int first)
{
	int	i;

	entity->f_time = ANIMATION_SPEED;
	entity->frames = ft_calloc((n + 1), sizeof(t_txtr));
	if (!entity->frames)
		return (error(data, E_MALLOC, NULL));
	i = 0;
	while (i < n)
	{
		entity->frames[i] = &data->txtr[first + i];
		++i;
	}
	entity->frames[i] = NULL;
}
