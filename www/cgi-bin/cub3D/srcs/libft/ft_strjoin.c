/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edraccan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 12:13:31 by edraccan          #+#    #+#             */
/*   Updated: 2025/04/12 10:00:08 by edraccan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*join;
	int		i;
	int		j;

	i = 0;
	j = 0;
	join = (char *)malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (!join)
		return (NULL);
	while (s1 && s1[i] != '\0')
		join[j++] = s1[i++];
	i = 0;
	while (s2 && s2[i] != '\0')
		join[j++] = s2[i++];
	join[j] = '\0';
	return (join);
}

/* 
int main()
{
	char *s = ft_strjoin("ciao", "mondo");
	printf("|%s|\n", s);
	free(s);
} */