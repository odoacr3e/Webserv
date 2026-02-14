/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_rejoin.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lparolis <lparolis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 11:52:40 by lparolis          #+#    #+#             */
/*   Updated: 2025/04/08 15:57:36 by lparolis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_rejoin(char *orig, char *new, int flag1, int flag2)
{
	char	*ret;

	if (!new && !orig)
		return (NULL);
	if (!orig)
	{
		ret = ft_strdup(new);
		if (new && flag2 == 1)
			free(new);
		return (ret);
	}
	if (!new)
		return (ft_strdup(orig));
	ret = ft_strjoin(orig, new);
	if (orig && flag1 == 1)
		free(orig);
	if (new && flag2 == 1)
		free(new);
	return (ret);
}
