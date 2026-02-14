/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_overflow.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alerusso <alerusso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 15:04:20 by alerusso          #+#    #+#             */
/*   Updated: 2025/08/21 14:55:36 by alerusso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static long long int	ft_pow(long long int n, int p)
{
	int	num;

	num = n;
	p -= p == 19;
	if (n == 0 || p <= 0)
		return (1);
	while (p-- > 1)
		n *= num;
	return (n);
}

/*
//REVIEW - overflow_check

	Checks if the number in the string overflows the given limits.
	Returns 1 if it overflows, 0 if it does not.

//	1)	We check if the string is empty;
	2)	i is set to 1 if str[0] is '-' or '+';
	3)	We increase limit_size until the limit is less than 10;
	4)	We skip the leading zeros;
	5)	num_size is set to the number of digits in the string.
		Sign, leading zeroes are skipped;
	6)	We scroll the string until the current char is a digit;
	7)	If the current char is not a digit, there is an illegal
		char: therefore we return 1.
		If num_size is different from limit_size:
		if it's greater, there's an overflow. We return 1;
		If it's smaller, we return 0;
	8)	If num_size is equal to limit_size, we check every digit
		of the string and every digit of the limit: we stop
		when a different value is found;
	9)	If the current char is greater than the current limit
		digit, we return 1. Else, 0 is returned.
*/
int	overflow_check(char *s, long long max, long long min, int limit_size)
{
	long long int	limit;
	int				i;
	int				num_size;

	if (!s || !s[0])
		return (true);
	limit = max * (s[0] != '-') + min * (s[0] == '-');
	i = ((s[0] == '-') || (s[0] == '+'));
	max = limit;
	while (++limit_size && (limit > 9 || limit < -9))
		limit /= 10;
	limit = max;
	while (s[i] == '0')
		++i;
	num_size = ft_strlen(s + i);
	while (ft_isdigit(s[i]))
		++i;
	if (s[i] || num_size != limit_size)
		return (s[i] || num_size > limit_size);
	i -= num_size + 1;
	while (++i && --num_size && s[i] == limit / ft_pow(10, num_size) % 10 * \
(-1 * (s[0] == '-') + 1 * (s[0] != '-')) + '0')
		;
	return (s[i] > limit / ft_pow(10, num_size) % 10 * \
(-1 * (s[0] == '-') + 1 * (s[0] != '-')) + '0');
}
