/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alerusso <alerusso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 14:28:07 by alerusso          #+#    #+#             */
/*   Updated: 2025/08/28 15:41:28 by alerusso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cub3D_bonus.h"

/*
	Returns the result of a division between doubles.
	Assures no division for 0 is made, avoiding floating point exception.
*/
double	safe_division(double delta, double sum)
{
	if (sum == 0 || delta == 0)
		return (0);
	return (delta / sum);
}

void	normalize_angle(double *angle)
{
	if (*angle > 2 * PI)
		*angle -= 2 * PI;
	else if (*angle < 0)
		*angle += 2 * PI;
}

/*
	Round a double value.
	Used because math.h round often fails.
*/
double	round_rad(double rad)
{
	if (rad > 0 && rad < 0.0001)
		return (0);
	else if (rad > 0 && rad > 0.9999)
		return (1);
	else if (rad < 0 && rad > -0.0001)
		return (0);
	else if (rad < 0 && rad < -0.9999)
		return (-1);
	return (rad);
}

int	bigger(int a, int b, int c, int d)
{
	int	bigger;

	bigger = a;
	if (b > bigger)
		bigger = b;
	if (c > bigger)
		bigger = c;
	if (d > bigger)
		bigger = c;
	return (bigger);
}

int	get_rand(int max)
{
	struct timeval	t;

	gettimeofday(&t, NULL);
	return (t.tv_usec % max);
}
