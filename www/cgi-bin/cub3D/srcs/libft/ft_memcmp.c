/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alerusso <alessandro.russo.frc@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 15:18:26 by edraccan          #+#    #+#             */
/*   Updated: 2026/02/10 18:09:21 by alerusso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t			i;
	unsigned char	*str1;
	unsigned char	*str2;

	i = 0;
	str1 = (void *)s1;
	str2 = (void *)s2;
	while (str1[i] != '\0' && i < n)
	{
		if (str1[i] != str2[i])
			return (str1[i] - str2[i]);
		i++;
	}
//	if (i == 0 && str1[i] == '\0')
//		return (str1[i] - str2[i]);
	return (0);
}

/* 
int main()
{
	printf("%d\n", ft_memcmp("abcdefghij", "abcdefgxyz", 7));
	printf("%d\n", memcmp("abcdefghij", "abcdefgxyz", 7));
} */