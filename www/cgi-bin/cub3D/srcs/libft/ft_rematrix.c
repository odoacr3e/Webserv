/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rematrix.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lparolis <lparolis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 11:28:43 by lparolis          #+#    #+#             */
/*   Updated: 2025/04/11 12:46:59 by lparolis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	**ft_rematrix(char **old, char **new)
{
	char	**ret;

	ret = ft_dup_matrix(new);
	free_matrix(old);
	free_matrix(new);
	return (ret);
}
