/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_restr.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alerusso <alerusso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 18:03:44 by edraccan          #+#    #+#             */
/*   Updated: 2025/06/11 20:57:44 by alerusso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// restituisce un dup della stringa nuova freeando old
char	*ft_restr(char *old, char *new)
{
	free(old);
	return (new);
}
