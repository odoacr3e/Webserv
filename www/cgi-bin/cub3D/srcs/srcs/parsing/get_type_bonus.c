/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_type_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lparolis <lparolis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 17:31:35 by lparolis          #+#    #+#             */
/*   Updated: 2025/09/01 11:42:36 by lparolis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cub3D_bonus.h"

static bool	assign_type(t_data *data, char *line);
static bool	cub3d_substr(t_data *data, char *line, int type);
static bool	end_line_check(t_data *data, char **line, int fd);

/* REVIEW
Funzione che cerca all'interno del file .cub passato come argomento
i vari type identifiers per le varie textures e eventualmente le controlla
usando le altre funzioni
 */
void	get_type(t_data *data, int fd)
{
	int			count_types;
	char		*line;
	static int	i;

	count_types = 0;
	line = get_next_line(fd);
	while (count_types != TYPE_IDENTIFIERS_NUM && line)
	{
		i += sub_strlen(line, " ", INCLUDE);
		if (line[i] == '\0')
			return (finish_him(fd), free(line), close(fd), \
error(data, E_TYPE, NULL));
		else if (line[i] == '\n')
		{
			line = ft_restr(line, get_next_line(fd));
			continue ;
		}
		if (assign_type(data, line + i) == false)
			return (finish_him(fd), free(line), close(fd), \
error(data, E_TYPE, NULL));
		++count_types;
		line = ft_restr(line, get_next_line(fd));
	}
	if (count_types != 6 || end_line_check(data, &line, fd) == false)
		return (finish_him(fd), close(fd), error(data, E_TYPE, NULL));
}

/*
	Skips first characters (the identifier) and assign the line
	in the correct ptr.
	If a ptr is already allocated, returns error.
*/
static bool	assign_type(t_data *data, char *line)
{
	if (ft_strncmp(line, "NO ", 2) == 0)
		return (cub3d_substr(data, line + 3, NO));
	else if (ft_strncmp(line, "SO ", 2) == 0)
		return (cub3d_substr(data, line + 3, SO));
	else if (ft_strncmp(line, "WE ", 2) == 0)
		return (cub3d_substr(data, line + 3, WE));
	else if (ft_strncmp(line, "EA ", 2) == 0)
		return (cub3d_substr(data, line + 3, EA));
	else if (ft_strncmp(line, "F ", 1) == 0)
		return (cub3d_substr(data, line + 2, F));
	else if (ft_strncmp(line, "C ", 1) == 0)
		return (cub3d_substr(data, line + 2, C));
	return (false);
}

static bool	cub3d_substr(t_data *data, char *line, int type)
{
	while (ft_isspace(line[0]))
		line += 1;
	if (type == NO && data->txtr[NORTH].path == NULL)
		data->txtr[NORTH].path = ft_strdup(line);
	else if (type == SO && data->txtr[SOUTH].path == NULL)
		data->txtr[SOUTH].path = ft_strdup(line);
	else if (type == WE && data->txtr[WEST].path == NULL)
		data->txtr[WEST].path = ft_strdup(line);
	else if (type == EA && data->txtr[EAST].path == NULL)
		data->txtr[EAST].path = ft_strdup(line);
	else if (type == F && data->txtr_floor == NULL)
		data->txtr_floor = ft_strdup(line);
	else if (type == C && data->txtr_ceiling == NULL)
		data->txtr_ceiling = ft_strdup(line);
	else
		return (false);
	return (true);
}

/*
	checks that, after the parse of the identifiers, 
	there is an empty line.
*/
static bool	end_line_check(t_data *data, char **line, int fd)
{
	char	end_line;

	if (!*line)
		return (false);
	end_line = **line;
	free(*line);
	if (end_line == '\n')
		return (true);
	finish_him(fd);
	close(fd);
	error(data, E_FORMAT, NULL);
	return (false);
}
