/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edraccan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 18:49:10 by edraccan          #+#    #+#             */
/*   Updated: 2024/11/20 19:02:09 by edraccan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *s, size_t n)
{
	size_t			i;
	unsigned char	*p;

	i = 0;
	p = s;
	while (i < n)
	{
		p[i] = 0;
		i++;
	}
}

/* 
int main()
{
	char s[20] = "ciao mondo";
	ft_bzero(s, 5);
	int i = 0;
	while (s[i] == 0)
	{
		printf("0\n");
		i++;
	}
} */