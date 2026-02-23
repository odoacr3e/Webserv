/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edraccan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 15:52:05 by edraccan          #+#    #+#             */
/*   Updated: 2024/12/23 17:46:37 by edraccan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

void	set_buffer(char *line, char buf[BUFFER_SIZE + 1])
{
	int	i;
	int	j;
	int	tmp;

	i = 0;
	while ((line && line[i]) && line[i] != '\n')
		i++;
	j = 0;
	if (line && line[i] == '\0')
	{
		buf[0] = 0;
		return ;
	}
	i++;
	tmp = i;
	while (line && line[i])
		buf[j++] = line[i++];
	buf[j] = '\0';
	line[tmp] = '\0';
}

char	*get_new_line(int fd, char *line, char buffer[BUFFER_SIZE + 1])
{
	int		bytes;
	char	*tmp;
	char	*strtmp;

	bytes = 1;
	tmp = ft_strdup(buffer);
	while (bytes != 0)
	{
		bytes = read(fd, line, BUFFER_SIZE);
		if (bytes == -1 && !tmp[0])
			return (free(tmp), NULL);
		if (bytes == 0 && !tmp[0])
			break ;
		line[bytes] = '\0';
		strtmp = ft_strdup(tmp);
		free(tmp);
		tmp = ft_strjoin(strtmp, line);
		free(strtmp);
		if (ft_strchr(tmp, '\n') != NULL)
			break ;
	}
	return (tmp);
}

// /* NOTE
// ** mandatory function, set buffer to the new read line until it doesn't
// ** find a \n, after it will copy on line only the part of buffer before \n
// ** (included) and after it updates buffer to the values after \n and
// ** it returns line
// */
char	*get_next_line(int fd)
{
	static char	buffer[BUFFER_SIZE + 1];
	char		*line;
	char		*tmp;

	if (fd < 0 || BUFFER_SIZE < 1)
		return (NULL);
	tmp = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!tmp)
		return (NULL);
	line = get_new_line(fd, tmp, buffer);
	free(tmp);
	if (!line || !line[0])
		return (free(line), NULL);
	set_buffer(line, buffer);
	return (line);
}

// int main()
// {
// 	int fd = 1007777777777770;
// 	char *s = get_next_line(fd);
// 	printf("%s", s);
// 	free(s);
// 	s = get_next_line(fd);
// 	while (s)
// 	{
// 		printf("%s", s);
// 		free(s);
// 		s = get_next_line(fd);
// 	}
// 	free(s);
// 	close(fd);
// }