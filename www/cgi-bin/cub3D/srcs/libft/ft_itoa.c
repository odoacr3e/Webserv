/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edraccan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 15:45:32 by edraccan          #+#    #+#             */
/*   Updated: 2024/11/26 15:05:53 by edraccan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	numlen(int n)
{
	int	i;

	i = 0;
	while (n > 0)
	{
		n /= 10;
		i++;
	}
	return (i);
}

static void	min_int(char *s)
{
	s[1] = '2';
	s[2] = '1';
	s[3] = '4';
	s[4] = '7';
	s[5] = '4';
	s[6] = '8';
	s[7] = '3';
	s[8] = '6';
	s[9] = '4';
	s[10] = '8';
	s[11] = '\0';
}

static void	check_number(char *s, int n, int j)
{
	char	tmp[12];
	int		i;

	i = 0;
	if (n < 0)
	{
		s[0] = '-';
		if (n == -2147483648)
		{
			min_int(s);
			return ;
		}
		n *= -1;
		i = 1;
	}
	while (n > 0)
	{
		tmp[j++] = (n % 10) + 48;
		n /= 10;
	}
	tmp[j--] = '\0';
	while (tmp[j])
		s[i++] = tmp[j--];
	s[i] = '\0';
}

char	*ft_itoa(int n)
{
	char	*s;

	if (n > 0)
		s = (char *)malloc(sizeof(char) * (numlen(n) + 1));
	else if (n < 0)
		s = (char *)malloc(sizeof(char) * (numlen(-n) + 2));
	else
	{
		s = (char *)malloc(sizeof(char) * 2);
		s[0] = '0';
		s[1] = '\0';
		return (s);
	}
	check_number(s, n, 0);
	return (s);
}

/* 
int main()
{
	char *s = ft_itoa(-2147483648);
	printf("itoa: %s\n", s);
	free(s);
} */