/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edraccan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 13:58:15 by edraccan          #+#    #+#             */
/*   Updated: 2024/11/24 11:40:58 by edraccan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	check_c(char *str, int c)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == c)
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strrchr(const char *s, int c)
{
	int		i;
	char	*str;

	i = 0;
	str = (char *)s;
	while (str[i] != '\0')
	{
		if (str[i] == c)
		{
			if (check_c(&str[i + 1], c) == 0)
				return (&str[i]);
		}
		i++;
	}
	if (s[i] == c)
		return (&str[i]);
	return (NULL);
}

/* 
int main()
{
	char a[30] = "abcdefaghijklmnoapqrstuvwxyz";
	printf("%s\n", ft_strrchr(a, 'a'));
} */