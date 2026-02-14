/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lparolis <lparolis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 17:01:33 by lparolis          #+#    #+#             */
/*   Updated: 2024/12/21 17:01:33 by lparolis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 20
# endif
# ifndef MAX_FD
#  define MAX_FD 1024
# endif
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdio.h>
# include <stdbool.h>

char		*get_strchr(char *s, char c);
char		*ft_schiavista(int fd, char *statik, char *buffer);
char		*get_substr(char *s, unsigned int start, size_t len);
char		*get_strjoin(char *s1, char *s2);
char		**gnl_statik(int fd, int reset);
char		*ft_cacatore(char *line);
char		*get_next_line(int fd);
char		*get_strdup(char *s);
size_t		get_strlen(char *s);

#endif