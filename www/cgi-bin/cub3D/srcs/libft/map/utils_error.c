/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alerusso <alessandro.russo.frc@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 23:43:06 by alerusso          #+#    #+#             */
/*   Updated: 2026/02/10 14:17:28 by alerusso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "map_internal.h"

int	__map_error(t_map *ptr, int code, void *data, const char *caller)
{
	MAP_LOG("Map Error:");
	if (code > MAP_SECTION_CRITICAL)
	{
		MAP_LOG("\033[31m Critical Error\033[0m");
		ptr->fail = true;
		map_clear(ptr);
	}
	MAP_LOG(" in function *%s*\n", caller);
	switch (code)
	{
	case MAP_OK:
		MAP_SWITCH_LOG("No errors found");
	case MAP_EXIST:
		MAP_SWITCH_LOG("Key %s already exist", (char*)data);
	case MAP_NOTFOUND :
		MAP_SWITCH_LOG("Key %s not found", (char*)data);
	case MAP_BUG:
		MAP_SWITCH_LOG("Bug: %s", (char*)data);
	case MAP_EINVAL:
	case MAP_INIT_EINVAL:
		MAP_SWITCH_LOG("Bad parameter");
	case MAP_NOMEM:
		MAP_SWITCH_LOG("Allocation failure");
	}
	MAP_LOG("\n");
	return (code);
}
