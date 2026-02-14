/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lparolis <lparolis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 16:48:58 by lparolis          #+#    #+#             */
/*   Updated: 2024/12/21 16:48:58 by lparolis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*get_strdup(char *s1)
{
	char			*dest;
	unsigned int	i;

	dest = (char *) malloc(get_strlen(s1) + 1);
	if (!dest)
		return (NULL);
	i = 0;
	while (s1[i])
	{
		dest[i] = s1[i];
		i++;
	}
	dest[i] = 0;
	return (dest);
}

size_t	get_strlen(char *s)
{
	int	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

char	*get_substr(char *s, unsigned int start, size_t len)
{
	size_t	i;
	char	*str;

	if (!s)
		return (NULL);
	if (start > get_strlen(s))
		return (malloc(1));
	if (len > get_strlen(s + start))
		len = get_strlen(s + start);
	str = malloc((len + 1) * sizeof(char));
	if (!str)
		return (NULL);
	i = 0;
	while (i < len)
	{
		str[i] = s[start + i];
		i++;
	}
	str[i] = 0;
	return (str);
}

char	*get_strjoin(char *s1, char *s2)
{
	char			*res;
	unsigned int	i;
	unsigned int	j;

	res = (char *) malloc((get_strlen(s1) + get_strlen(s2) + 1) * sizeof(char));
	if (!res)
		return (NULL);
	i = 0;
	j = 0;
	while (s1[j])
		res[i++] = s1[j++];
	j = 0;
	while (s2[j])
		res[i++] = s2[j++];
	res[i] = '\0';
	return (res);
}

char	**gnl_statik(int fd, int reset)
{
	static char	*statik[MAX_FD];

	if (fd < 0 || fd >= MAX_FD)
		return (NULL);
	if (reset)
	{
		free(statik[fd]);
		statik[fd] = NULL;
	}
	return (&statik[fd]);
}
