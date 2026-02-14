/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_bfs.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alerusso <alerusso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 13:58:05 by alerusso          #+#    #+#             */
/*   Updated: 2025/06/10 16:42:27 by alerusso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GAME_BFS_H
# define GAME_BFS_H
# include "libft.h"

typedef struct s_typelist	t_typelist;

/*
	PREREQUISITES:
	1)	Data structure allocated on heap/stack 
		(heap is better);
	2)	The struct MUST have:
		-	The coordinates of the enemy;
		-	The coordinates of the player;
		-	The size of the map;
		-	A special struct called position, that
			you must use to store your map values.
	3)	The map you give MUST be surronded by walls
		(in this version).
	4)	Go to "game_bfs.c", and modify the bottom
		function "get_best_path": you must make it
		switch the data listed on 2) to the t_bfs
		struct.
	
	On success, every position[x][y].distance[en_num]
	will be filled with 1 and 0.
	The enemies will go to where they see the one.

	On success 0 will be returned.
	On fail, another error int.

	TO TEST THE ALGORYTHM, compile "game_bfs_copy.c"
	with the libft functions (with the list support).
*/

typedef struct s_position
{
	unsigned char	value;
	char			*distance;
}				t_position;

typedef struct s_bfs
{
	int			n;
	int			mark;
	int			en_x;
	int			en_y;
	int			p_x;
	int			p_y;
	int			x;
	int			y;
	int			map_x;
	int			map_y;
	t_position	**position;
	int			distance;
}				t_bfs;

t_typelist	*store_list(t_typelist *list, int mode);
void		del_free(void *ptr);
int			draw_path(t_bfs *bfs);
void		bfs(t_bfs *bfs);
void		clean_bfs(t_bfs *bfs, int enemy_num);
//int	    get_best_path(t_map *map, int enemy_num);

#endif