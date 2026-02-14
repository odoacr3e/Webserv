/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edraccan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 19:05:14 by edraccan          #+#    #+#             */
/*   Updated: 2024/11/26 14:51:36 by edraccan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t			i;
	unsigned char	*d;

	i = 0;
	d = dest;
	while (i < n)
	{
		d[i] = *((unsigned char *)src + i);
		i++;
	}
	return (dest);
}

/* 
int main()
{
	char message2[60] = "abcdef";
	char temp[60];
	strcpy(temp, message2);

	ft_memcpy(message2 + 1, message2, 5);
	printf("ft: %s\n", temp);
	memcpy(temp + 1, temp, 5);
	printf("no ft: %s\n", temp);
} */