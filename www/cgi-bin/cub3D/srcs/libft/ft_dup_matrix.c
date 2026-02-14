/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dup_matrix.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edraccan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 11:31:08 by lparolis          #+#    #+#             */
/*   Updated: 2025/04/26 13:27:10 by edraccan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	**ft_dup_matrix(char **matrix)
{
	int		i;
	char	**new;

	new = malloc(sizeof(char *) * (matrix_len(matrix) + 1));
	if (!new)
		return (NULL);
	i = 0;
	while (matrix && matrix[i])
	{
		new[i] = ft_strdup(matrix[i]);
		i++;
	}
	new[i] = NULL;
	return (new);
}
