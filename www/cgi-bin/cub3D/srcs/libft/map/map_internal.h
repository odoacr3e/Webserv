/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_internal.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alerusso <alessandro.russo.frc@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 21:55:16 by alerusso          #+#    #+#             */
/*   Updated: 2026/02/10 14:16:34 by alerusso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAP_INTERNAL_H
# define MAP_INTERNAL_H

# include "map.h"

# define MAP_NPOS (size_t)-1
# if MAP_LOG_BOOL == true
#  define MAP_SWITCH_LOG(s, ...) printf(s, ##__VA_ARGS__);break 
#  define MAP_LOG(s, ...) printf(s, ##__VA_ARGS__)
# else
#  define MAP_SWITCH_LOG(s, ...) (void)0;break 
#  define MAP_LOG(s, ...) (void)0
# endif

typedef struct s_map		t_map;
typedef struct s_map_values	t_map_values;
typedef struct s_map_val	t_map_val;

typedef struct s_map_find_data
{
    size_t	    i;
    t_list      *node;
    t_list      *head;
	t_map_val	pair;
}	t_map_find_val;

# define _map_error(code, data)	__map_error(ptr, code, data, __FUNCTION__)

int		_map_default_alloc(t_map_val *pair, char *key, void *val);
void	_map_default_del(t_map_val *pair);
size_t	_map_default_hash(const char *key);
int		__map_error(t_map *ptr, int code, void *data, const char *caller);
int		_map_insert(t_map *ptr, t_map_find_val *val);
bool	_map_find(t_map *ptr, const char *key, void *new_val, t_map_find_val *val);
void	_map_set_key_val(t_map_find_val *val);
int 	_map_add(t_map *ptr, t_map_find_val *data);
int 	_map_replace(t_map *ptr, t_map_find_val *data);
void 	*_map_get(t_map *ptr, t_map_find_val *data);
void 	_map_del(t_map *ptr, t_map_find_val *data);

#endif