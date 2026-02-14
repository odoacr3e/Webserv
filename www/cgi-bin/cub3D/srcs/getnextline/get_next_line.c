/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lparolis <lparolis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 17:57:58 by lparolis          #+#    #+#             */
/*   Updated: 2024/12/21 17:57:58 by lparolis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*get_strchr(char *s, char c)
{
	unsigned int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == c)
			return ((char *) &s[i]);
		i++;
	}
	if (s[i] == c)
		return ((char *) &s[i]);
	return (NULL);
}

char	*ft_schiavista(int fd, char *statik, char *buffer)
{
	ssize_t	read_bytes;
	char	*temp;

	read_bytes = 1;
	while (read_bytes > 0)
	{
		read_bytes = read(fd, buffer, BUFFER_SIZE);
		if (read_bytes == -1)
		{
			free(statik);
			return (NULL);
		}
		if (read_bytes == 0)
			break ;
		buffer[read_bytes] = 0;
		if (!statik)
			statik = get_strdup("");
		temp = statik;
		statik = get_strjoin(temp, buffer);
		free(temp);
		temp = NULL;
		if (get_strchr(buffer, '\n'))
			break ;
	}
	return (statik);
}

char	*ft_cacatore(char *line)
{
	char	*leftover;
	ssize_t	counter;

	counter = 0;
	while (line[counter] != '\n' && line[counter] != '\0')
		counter++;
	if (line[counter] == 0 || line[counter + 1] == 0)
		return (NULL);
	leftover = get_substr(line, counter + 1, get_strlen(line) - counter);
	if (*leftover == 0)
	{
		free(leftover);
		leftover = NULL;
	}
	line[counter + 1] = 0;
	return (leftover);
}

char	*get_next_line(int fd)
{
	char		*result;
	char		*buffer;
	char		**statik;

	statik = gnl_statik(fd, false);
	buffer = (char *)malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (fd < 0 || BUFFER_SIZE <= 0)
	{
		gnl_statik(fd, true);
		free(buffer);
		buffer = NULL;
		return (NULL);
	}
	if (!buffer)
		return (NULL);
	result = ft_schiavista(fd, *statik, buffer);
	free(buffer);
	buffer = NULL;
	if (!result)
		return (NULL);
	*statik = ft_cacatore(result);
	return (result);
}

/* int main ()
{
	char	*line;
	int		fd;
	int		i = 1;  

	fd = open("test.txt", O_RDONLY);
	line = get_next_line(fd);
	while (1)
	{
		if (line == NULL)
		{
			printf("	[FINE]	");
			break ;
		}
		printf("riga %d: %s", i, line);
		free(line);
		line = get_next_line(fd);
		i++;
	}
	free(line);
	close(fd);
	return (0);
} */