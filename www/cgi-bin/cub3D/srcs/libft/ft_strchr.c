/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edraccan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 12:32:26 by edraccan          #+#    #+#             */
/*   Updated: 2025/04/01 17:23:04 by edraccan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	int		i;
	char	*str;

	i = 0;
	if (!s)
		return (NULL);
	str = (char *)s;
	while (s[i] != '\0')
	{
		if (s[i] == c)
			return (&str[i]);
		i++;
	}
	if (s[i] == c)
		return (&str[i]);
	return (NULL);
}

/* 
int main()
{
	char a[30] = "abcdefghijklmnopqrstuvwxyz";
	printf("%s\n", ft_strchr(a, 'd'));
} */