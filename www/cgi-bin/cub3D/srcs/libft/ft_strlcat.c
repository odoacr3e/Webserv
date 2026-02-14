/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edraccan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 10:47:47 by edraccan          #+#    #+#             */
/*   Updated: 2025/03/28 19:19:25 by edraccan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/* NOTE
** return = len(dst) + len(src) se size - len(dst) - 1 se >= 0
** altrimenti return = size + len(src)
*/

	/* size_t	i;
	size_t	r;

	if (!dst)
		return (0);
	i = 0;
	while (*dst && i < size)
	{
		++dst;
		++i;
	}
	r = ft_strlcpy(dst, src, size - i);
	return (r + i); */

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	dstl;
	size_t	srcl;
	size_t	i;
	size_t	j;

	dstl = ft_strlen(dst);
	srcl = ft_strlen(src);
	if (size <= dstl)
		return (size + srcl);
	i = dstl;
	j = 0;
	while (src[j] && i < size - 1)
	{
		dst[i] = src[j];
		j++;
		i++;
	}
	dst[i] = '\0';
	return (dstl + srcl);
}

/* 
int main(void)
{
	char *a = (char *)malloc(sizeof(char) * 20);
	char *a2 = (char *)malloc(sizeof(char) * 20);
	int     l;
	
	a[11] = 'a';
	printf("a: %s\n", a);
	l = ft_strlcat(a,"lorem",15);
	printf("ft_strlcat: %d\n", l);
	printf("the string: %s\n", a);
	
	a2[11] = 'a';
	printf("a2: %s\n", a2);
	l = strlcat(a2,"lorem",15);
	printf("strlcat: %d\n", l);
	printf("the string: %s\n", a2);
	return (0);
} */