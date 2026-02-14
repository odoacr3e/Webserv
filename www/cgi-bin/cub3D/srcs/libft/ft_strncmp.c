/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alerusso <alessandro.russo.frc@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 14:45:38 by edraccan          #+#    #+#             */
/*   Updated: 2026/02/10 15:31:58 by alerusso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* NOTE
** return 0 if all characters of both strings are equals
*/
int	ft_strncmp(const char *s1, const char *s2, size_t n)
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
		}
		else
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
	}
//	if (s1[i] != s2[i])
//		return ((unsigned char)s1[i] - (unsigned char)s2[i]);
	return (0);
}

/* 
int main()
{
	printf("ft: %d\n", ft_strncmp("test", "testss", 7));
	printf("%d\n", strncmp("test", "testss", 7));
} */