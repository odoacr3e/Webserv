/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix_longest_line.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lparolis <lparolis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 12:04:59 by lparolis          #+#    #+#             */
/*   Updated: 2025/06/12 12:14:17 by lparolis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	matrix_longest_line(char **matrix)
{
	size_t	size;
	size_t	temp;
	int		i;

	temp = 0;
	i = -1;
	size = 0;
	while (matrix && matrix[++i])
	{
		temp = ft_strlen(matrix[i]);
		if (temp > size)
			size = temp;
	}
	return (size);
}
