/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edraccan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 09:49:05 by edraccan          #+#    #+#             */
/*   Updated: 2025/03/28 19:19:27 by edraccan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	i;

	i = 0;
	if (!dst || size <= 0)
		return (ft_strlen(src));
	while (src[i] != '\0' && i < size - 1)
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (ft_strlen(src));
}

/* 
int main()
{
	char src[50] = "lorem ipsum dolor sit amet";
	char dst[50];
	char dst2[50];

	printf("ft: %zu, %s\n", ft_strlcpy(dst, src, 0), dst);
	printf("no ft: %zu, %s\n", strlcpy(dst2, src, 0), dst);
} */