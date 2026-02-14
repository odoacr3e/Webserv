/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lparolis <lparolis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 16:20:20 by edraccan          #+#    #+#             */
/*   Updated: 2025/05/05 18:19:05 by lparolis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp_2(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	if (n == 0)
		return (0);
	if (!s1 || !s2)
		return (-1);
	i = 0;
	while (i < n)
	{
		if (s1[i] && s1[i] == s2[i])
		{
			while (s1[i] && s1[i] == s2[i] && i < n)
				i++;
			if (s1[i] && s1[i] == '\n')
				return (0);
			if (s1[i] && s1[i] != s2[i])
				return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		}
		else
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
	}
	return (0);
}
