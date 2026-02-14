/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alerusso <alessandro.russo.frc@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 14:48:55 by alerusso          #+#    #+#             */
/*   Updated: 2025/08/23 14:49:19 by alerusso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cub3D_bonus.h"

int	count_chars(t_data *data, int *count, char *search)
{
	int	i;
	int	j;

	i = -1;
	while (data->map && data->map[++i])
	{
		j = -1;
		while (data->map && data->map[i][++j])
		{
			if (ft_strchr(VALID_CHARS, data->map[i][j]) == NULL)
				return (error(data, E_CHAR, NULL), -1);
			if (ft_strchr(search, data->map[i][j]))
				(*count)++;
		}
	}
	return (*count);
}
