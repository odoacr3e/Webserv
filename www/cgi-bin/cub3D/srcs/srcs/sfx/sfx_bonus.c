/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sfx_bonus.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alerusso <alessandro.russo.frc@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 14:54:57 by alerusso          #+#    #+#             */
/*   Updated: 2026/02/10 16:28:11 by alerusso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3D_bonus.h"

void	stop_audio(t_data *data)
{
	if (data->audio_play && data->audio_pid > 0)
	{
		kill(data->audio_pid, SIGTERM);
		data->audio_pid = 0;
		data->audio_play = false;
	}
}

void	play_audio(char *audio_path, t_data *data)
{
	FILE	*fd;
	int		bytes;
	char	command[256];

	stop_audio(data);
	snprintf(command, sizeof(command), "paplay %s > /dev/null 2>&1 & echo $!", \
audio_path);
	fd = popen(command, "r");
	if (fd)
	{
		bytes = fscanf(fd, "%d", &data->audio_pid);
		(void)bytes;
		pclose(fd);
		data->audio_play = true;
	}
}
