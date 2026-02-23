/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edraccan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 15:52:16 by edraccan          #+#    #+#             */
/*   Updated: 2025/04/08 16:38:57 by edraccan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1000
# endif

# include <unistd.h>
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>

/* SECTION
** get_next_line.c
*/
void	set_buffer(char *line, char buf[BUFFER_SIZE + 1]);
char	*get_new_line(int fd, char *line, char buffer[BUFFER_SIZE + 1]);
char	*get_next_line(int fd);

/* SECTION
** get_next_line_utils.c
*/
char	*ft_strdup(char *s);
char	*ft_strchr(const char *s, int c);
int		ft_strlen(char *s);
char	*ft_strjoin(char *s1, char *s2);

#endif