/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strings.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alerusso <alerusso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 16:16:11 by alerusso          #+#    #+#             */
/*   Updated: 2025/08/21 14:54:45 by alerusso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*REVIEW - cat_string

//Cat a string in another string, starting from start (included).
*/
static void	to_free(char *s1, char *s2, int which);

// usage: new = _cat_string(src, catstr, 4, 2);
//	src = "Hello";	catstr = " World"; start = 5; which_free = 2;
//	new = "Hello World"
//
//	which_free: 0 FREE NOTHING, 1 FREE src, 2 FREE catstr, 3 FREE BOTH	
char	*paste_string(char *src, char *catstr, size_t start, int which_free)
{
	size_t	i;
	char	*new;
	size_t	catstr_len;

	if (!src || !catstr)
		return (to_free(src, catstr, which_free), NULL);
	i = 0;
	while (src[i] && i < start)
		++i;
	if (i > start && !src[i])
		return (to_free(src, catstr, which_free), NULL);
	catstr_len = ft_strlen(catstr);
	new = (char *)ft_calloc(ft_strlen(src) + catstr_len + 2, sizeof(char));
	if (new)
	{
		ft_strlcpy(new, src, start + 2);
		ft_strlcpy(new + start, catstr, catstr_len + 2);
		if (src[i])
			sub_strcpy(new + start + catstr_len, src + start, "", EXCLUDE);
	}
	return (to_free(src, catstr, which_free), new);
}

static void	to_free(char *s1, char *s2, int which)
{
	if (which == 1 || which >= 3)
		free(s1);
	if (which == 2 || which >= 3)
		free(s2);
}

/*
	Cut a string from start to end (included).
	
	Example:

	string = "Hello world!"
	start = 5
	end = 5

	string becomes: "Helloworld!\0"
*/
int	cut_string(char *string, size_t start, size_t end)
{
	unsigned int	temp;
	unsigned int	temp1;
	unsigned int	string_len;

	end++;
	if (!(string) || !(*string) || (start > end))
		return (1);
	string_len = 0;
	while (string[string_len])
		++string_len;
	temp = start;
	while ((start != end) && (string[start] != 0))
		string[start++] = 0;
	end = start;
	start = temp;
	temp = 0;
	temp1 = string_len;
	while (end != string_len--)
	{
		string[start + temp] = string[end + temp];
		++temp;
	}
	string[temp1 - (end - start)] = 0;
	return (0);
}

// restituisce la lunghezza fino alla prima occorenza del 
// primo carattere che gli passi
int	sub_strlen(char *s, char *charset, int mode)
{
	int	i;

	i = 0;
	if (mode == INCLUDE)
	{
		while (((s[i] != '\0')) && \
(ft_strchr(charset, s[i])))
		{
			++i;
		}
	}
	else if (mode == EXCLUDE)
	{
		while (((s[i] != '\0')) && \
!(ft_strchr(charset, s[i])))
		{
			++i;
		}
	}
	return (i);
}

/*
	Copy until the char is in the charset.
	TWO MODES:

	INCLUDE: Copy until the char is in the charset.
	EXCLUDE: Copy until the char is NOT in the charset.
*/
int	sub_strcpy(char *dest, char *src, char *charset, int mode)
{
	int	i;

	i = 0;
	if (mode == INCLUDE)
	{
		while (((src[i] != '\0')) && \
(ft_strchr(charset, src[i])))
		{
			dest[i] = src[i];
			++i;
		}
	}
	else if (mode == EXCLUDE)
	{
		while (((src[i] != '\0')) && \
!(ft_strchr(charset, src[i])))
		{
			dest[i] = src[i];
			++i;
		}
	}
	dest[i] = 0;
	return (i);
}
