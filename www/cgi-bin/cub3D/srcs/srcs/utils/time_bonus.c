/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alerusso <alerusso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 09:55:18 by alerusso          #+#    #+#             */
/*   Updated: 2025/08/29 09:44:16 by alerusso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cub3D_bonus.h"

static long long	get_time(t_timecode timecode);

/*
	Brutal force CPU draining sleep.
*/
void	ft_sleep(long long microsecond)
{
	long long	start;
	long long	current;

	start = get_time(MICROSECONDS);
	current = start;
	while (current - start < microsecond)
	{
		current = get_time(MICROSECONDS);
	}
}

/*
	Returns time in integer format.
*/
static long long	get_time(t_timecode timecode)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
		fd_printf(2, "gettimeofday failed \n");
	if (timecode == SECONDS)
		return (tv.tv_sec + (tv.tv_usec / 1e6));
	else if (timecode == MILLISECONDS)
		return ((tv.tv_sec * 1e3) + (tv.tv_usec / 1e3));
	else if (timecode == MICROSECONDS)
		return ((tv.tv_sec * 1e6) + tv.tv_usec);
	else
		fd_printf(2, "Bad get_time call\n");
	return (123456);
}

//REVIEW - 	elapsed_time
/*
	Returns time passed, in microseconds, between now and start time.
	Start time is a struct timeval already set with gettimeofday.
	//FIXME : 10e6 instead of 1000000.
*/
long	elapsed_time(t_time start)
{
	struct timeval	tv;
	long			diff;

	gettimeofday(&tv, NULL);
	diff = (tv.tv_sec - start.tv_sec) * 1000000 \
+ (tv.tv_usec - start.tv_usec);
	return (diff);
}

bool	aspettanding(int time_to_wait, int i)
{
	static int	elapsed[100];
	int			index;

	if (i < 0)
	{
		index = -1;
		while (++index < 100)
			elapsed[index] = 0;
	}
	if (i >= 100)
		return (false);
	if (elapsed[i] < time_to_wait)
	{
		elapsed[i]++;
		return (false);
	}
	else
	{
		return (true);
	}
}
