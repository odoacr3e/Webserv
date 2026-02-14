/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alerusso <alessandro.russo.frc@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 21:55:16 by alerusso          #+#    #+#             */
/*   Updated: 2026/02/12 16:46:55 by alerusso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAP_H
# define MAP_H

# include "../libft.h"
# include <stdbool.h>

# define MAP_LOG_BOOL true
# define DEFAULT_SEED 42UL

typedef struct s_list		t_list;
typedef struct s_map		t_map;
typedef struct s_map_val	t_map_val;

/**
 *## struct s_map
 *### description
- stored data like an unsorted map.
- keys are unique. When adding a new key that already exists:
	`map_add`		returns error MAP_EXIST
	`map_replace`	clears old val if exists, then save new val
- hasher function can be set by changing the variable `hasher`
  default is murmurhash
- a delete function can be set by changing the variable `del`
  default: 	keys are freed with free
  			values are not freed
  EXAMPLE:
  void	del(t_map_val *pair){free(key), custom_free(val), free(pair);}
  map.del = del;
  the delete function is called when:
  `map_clear` method is called
  `map_erase` method is called
  a critical error occured (map->fail set)
- same for the alloc function
  EXAMPLE:
  int	alloc(t_map_val *pair, char *key, void *val)
  {
	pair->key = strdup(key), pair->val = custom_allocator(val);
	if (!pair->key || !pair->val){free(key), custom_free(val), return 1;}
	return 0;	
  }
  map.alloc = alloc;
 *### variables
`hasher`	a ptr to the function that hash the keys {size_t(*)(const char*)}
`alloc`		a ptr to the function that alloc t_map_val 
			{(t_map_val*, char *key, void *val))}
`del`		a ptr to the function that free t_map_val {void(*)(t_map_val*)}
`values`	an array of values (list of element that collides)
`size`		total number of element in the map
`hash_size`	size of the data array
`fail`		bool true if a critical error occured
 *### methods
 * |Action         |Function Prototype                                      |
 * |:--------------|:-------------------------------------------------------|
 * |:--------------|:-------------------------------------------------------|
 * | alloc data    | `int map_init(t_map *ptr, size_t hash_size)`		  	|
 * | free data     | `void map_free(t_map *ptr)`						  	|
 * | clear data    | `void map_clear(t_map *ptr)`						  	|
 * | key exists?   | `bool map_find(t_map *ptr, const char *key)`			|
 * | add key if new| `int map_add(t_map *ptr, char *key, void *new_val)`	|
 * | add key       | `int map_replace(t_map *ptr, char *key, void *new_val)`|
 * | get value     | `void *map_get(t_map *ptr, char *key)`			      	|
 * | delete key    | `void map_del(t_map *ptr, char *key)`				  	|
 * |:------------..|:-------------------------------------------------------|
 */
typedef struct s_map
{
	size_t(*hasher)(const char*);
	int(*alloc)(t_map_val*, char *key, void *val);
	void(*del)(t_map_val*);
	t_list			**values;
	size_t			size;
	size_t			hash_size;
	bool			fail;
}		t_map;

/*
-	accessed from t_map::values by keys
-	t_list *values store a list of t_map_val
-	these values' keys collide
-	key is the char * used to access t_map::values
-	val is the ptr of the data
*/
typedef struct s_map_val
{
	char	*key;
	void	*val;
}		t_map_val;

//do not touch these data
typedef struct s_map_private
{
	size_t		it;
}		t_map_private;

enum e_map_ret_codes
{
	MAP_OK,
	MAP_SECTION_ERROR,
	MAP_EXIST,
	MAP_NOTFOUND,
	MAP_EINVAL,
	MAP_BUG,
	MAP_SECTION_CRITICAL,
	MAP_INIT_EINVAL,
	MAP_NOMEM,
};

int 	map_init(t_map *ptr, size_t hash_size);
void 	map_free(t_map *ptr);
void 	map_clear(t_map *ptr);
bool	map_find(t_map *ptr, const char *key);
int 	map_add(t_map *ptr, char *key, void *new_val);
int 	map_replace(t_map *ptr, char *key, void *new_val);
void 	*map_get(t_map *ptr, char *key);
void 	map_del(t_map *ptr, char *key);


#endif