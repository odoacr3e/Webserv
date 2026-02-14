/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edraccan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 19:07:09 by edraccan          #+#    #+#             */
/*   Updated: 2024/11/25 18:42:55 by edraccan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	size_t			i;
	unsigned char	*d;

	i = 0;
	d = dest;
	if (dest <= src)
	{
		while (i < n)
		{
			d[i] = *((unsigned char *)src + i);
			i++;
		}
	}
	else
	{
		src += n - 1;
		while (n > 0)
		{
			d[n - 1] = *((unsigned char *)src--);
			n--;
		}
	}
	return (d);
}

/* 
int main()
{
	char message[60] = "abcdefghijklmnopqrstuvwxyz";
	char temp[60] = "abcdefghijklmnopqrstuvwxyz";

	ft_memmove(temp, temp + 2, 10);
	memmove(message, message + 2, 10);
	printf("ft: %s\n", temp);
	printf("  : %s\n", message);
} */