/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alerusso <alerusso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 17:07:04 by edraccan          #+#    #+#             */
/*   Updated: 2025/08/21 14:55:13 by alerusso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_words(char const *s, char c)
{
	int	i;
	int	wc;

	wc = 0;
	i = 0;
	while (s[i] == c)
		i++;
	while (s[i] != '\0')
	{
		if (s[i] == c && s[i - 1] != c)
			wc++;
		i++;
	}
	if (s[i] == '\0' && s[i - 1] != c && i > 0)
		wc++;
	return (wc);
}

static int	alloc_rows(char const *s, char c, char **ret, int i)
{
	int	len;

	len = 0;
	while (s[i] != '\0' && s[i] != c)
	{
		len++;
		i++;
	}
	ret[0] = ft_substr(&s[i - len], 0, len);
	return (i);
}

static char	**ft_free_matrix(char **ret)
{
	int	i;

	i = 0;
	while (ret[i])
	{
		free(ret[i]);
		i++;
	}
	free(ret);
	return (NULL);
}

char	**ft_split(char const *s, char c)
{
	char	**ret;
	int		i;
	int		j;

	i = 0;
	j = 0;
	ret = (char **)malloc(sizeof(char *) * ((count_words(s, c) * \
ft_strlen(s)) + 1));
	if (!ret)
		return (NULL);
	while (s[i] != '\0')
	{
		if (s[i] != c)
		{
			i = alloc_rows(s, c, &ret[j], i);
			if (!ret[j])
				return (ft_free_matrix(ret));
			j++;
		}
		else
			i++;
	}
	ret[j] = NULL;
	return (ret);
}

/* 
int main(void)
{
	char *s = "ciao a mondo";
	char **p = ft_split(s, ' ');
	int i = 0;
	while (p[i])
	{
		printf("%s\n", p[i]);
		free(p[i]);
		i++;
	}
	free(p);
} */