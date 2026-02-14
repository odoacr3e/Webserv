/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edraccan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 12:29:56 by edraccan          #+#    #+#             */
/*   Updated: 2024/11/25 17:28:10 by edraccan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	check_set_back(char const *s, char const *set)
{
	int	i;
	int	j;
	int	count;

	i = ft_strlen(s) - 1;
	count = 0;
	while (s[i] != '\0')
	{
		j = 0;
		while (set[j] != '\0' && set[j] != s[i])
			j++;
		if (j != (int)ft_strlen(set))
			count++;
		else
			return (count);
		i--;
	}
	return (count);
}

static int	check_set_front(char const *s, char const *set)
{
	int	i;
	int	j;
	int	count;

	i = 0;
	count = 0;
	while (s[i] != '\0')
	{
		j = 0;
		while (set[j] != '\0' && set[j] != s[i])
			j++;
		if (j != (int)ft_strlen(set))
			count++;
		else
			return (count);
		i++;
	}
	return (count);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*ret;
	int		i;
	int		j;
	int		cdelf;
	int		cdelb;

	cdelf = check_set_front(s1, set);
	cdelb = check_set_back(s1, set);
	i = 0;
	ret = (char *)malloc(sizeof(char) * (ft_strlen(s1) - cdelf + cdelb + 1));
	if (!ret)
		return (NULL);
	j = cdelf;
	while (i < (int)ft_strlen(s1) - cdelf - cdelb)
	{
		ret[i] = s1[j];
		i++;
		j++;
	}
	ret[i] = '\0';
	return (ret);
}

/* 
int main()
{
	char *s = "cipicppoci";
	printf("|%s|\n", ft_strtrim(s, "ic"));
	int len = ft_strlen("cipicppoci");
	if (s[len] == '\0')
		printf("\\0");
} */