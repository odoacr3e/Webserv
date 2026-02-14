/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_bfs_draw.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alerusso <alerusso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 14:02:21 by alerusso          #+#    #+#             */
/*   Updated: 2025/08/21 14:52:18 by alerusso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game_bfs.h"

/*
	-	Battle royal: we take a record equal to INT_MAX.
	-	We have four parameters: up, down, left, right.
	-	The winner will be the one with the lowest distance
		from enemy.

	-	If right is valid (> 0), it receives the record.
	-	If left is valid and less than right,
		 it receives the record.
	-	Now, we know the winner between right and left.
	-	If any of up or down are valid and beat the
		record, they get the record.

	-	At the end of this ranking process, we check
		what is the winner: we return the winner.
	-	If anything wins, it means that anything is valid.
		in this case, we return -1: it means there are
		no valid path.
*/
static int	find_shortest_route(t_bfs *bfs, int x, int y)
{
	int	record;

	record = INT_MAX;
	if (bfs->position[x + 1][y].distance[bfs->n] > 0)
		record = bfs->position[x + 1][y].distance[bfs->n];
	if ((bfs->position[x - 1][y].distance[bfs->n] > 0) && \
(bfs->position[x - 1][y].distance[bfs->n] < record))
		record = bfs->position[x - 1][y].distance[bfs->n];
	if ((bfs->position[x][y + 1].distance[bfs->n] > 0) && \
(bfs->position[x][y + 1].distance[bfs->n] < record))
		record = bfs->position[x][y + 1].distance[bfs->n];
	if ((bfs->position[x][y - 1].distance[bfs->n] > 0) && \
(bfs->position[x][y - 1].distance[bfs->n] < record))
		record = bfs->position[x][y - 1].distance[bfs->n];
	if (record == bfs->position[x + 1][y].distance[bfs->n])
		return (1);
	if (record == bfs->position[x - 1][y].distance[bfs->n])
		return (2);
	if (record == bfs->position[x][y + 1].distance[bfs->n])
		return (3);
	if (record == bfs->position[x][y - 1].distance[bfs->n])
		return (4);
	return (-1);
}

/*
	If any of the direction we can move is
	 the player's one, we stop.
*/
static int	found_end(int x, int y, int stop_x, int stop_y)
{
	if ((x + 1 == stop_x) && (y == stop_y))
		return (1);
	if ((x - 1 == stop_x) && (y == stop_y))
		return (1);
	if ((x == stop_x) && (y + 1 == stop_y))
		return (1);
	if ((x == stop_x) && (y - 1 == stop_y))
		return (1);
	return (0);
}

/*
	-	Here we put a bfs mark on the lowest
		 distance value.
	-	Notice that the mark is always a NEGATIVE number.
	-	Lastly, we move to it.
	-	rec stands for record. See "find shortest route" ft.
*/
static void	update_one(t_bfs *bfs, int *x, int *y, int rec)
{
	if (rec == 1)
	{
		bfs->position[*x + 1][*y].distance[bfs->n] = bfs->mark;
		(*x)++;
	}
	if (rec == 2)
	{
		bfs->position[*x - 1][*y].distance[bfs->n] = bfs->mark;
		(*x)--;
	}
	if (rec == 3)
	{
		bfs->position[*x][*y + 1].distance[bfs->n] = bfs->mark;
		(*y)++;
	}
	if (rec == 4)
	{
		bfs->position[*x][*y - 1].distance[bfs->n] = bfs->mark;
		(*y)--;
	}
}

/*
	-	Pad everything, except for NEGATIVE number (marked one),
		with zeros.
*/
static void	normalize_path(t_bfs *bfs, int map_x, int map_y)
{
	int			x;
	int			y;

	x = -1;
	y = -1;
	while (++y != map_y)
	{
		x = -1;
		while (++x != map_x)
		{
			if (bfs->position[x][y].distance[bfs->n] > 0)
				bfs->position[x][y].distance[bfs->n] = 0;
		}
	}
}

/*
	What are we doing here?

	-	At the end of the bfs process, every enemy will se
		something like this:

		-REAL MAP-			-WHAT ENEMIES SEE-

		111111111				000000000
		10$000011				01$123400
		100111001				021000560
		1000P0001				0345P7670
		111111111				000000000

	-	Actually, they can't recognize P and $.
		We need to tell them where they are.
	-	For everything else, this is what they see.
	-	What represents the numbers?
	-	The distances from the enemy, calculated with bfs.
	-	The zeroes are walls, or other unpassable objects.
	-	What we need to do now, is draw a clear path to the
		player. Every enemy has its own map to do so.
	-	In short words, we need to do this: 

		-FINAL PATH-

		000000000
		011000000
		010000000
		011110000
		000000000

	-	How do we do this?
	-	Simple: we need to find the littlest number
		from the player.
	-	In our example is 5: then we go to 5.
	-	Next, we have 4, and so on.
	-	We need to mark every number we want.
	-	We mark them with this formula: (n_enemy + 1) * -1.
	-	But, actually, we don't care how it is marked.
	-	What is important is that the enemy sees only two
		symbols: where it needs to go, and where it mustn't.
	-	After have marked the path (with negative numbers), 
		we set to zero every positive number.
	-	In this way, they'll just see the marks and the zeros.

	-	What happens if there is no valid path?
	-	It means that around player there is nothing 
		different from zeros.
	-	Then, we can just simply call the function that clears
		what enemy sees.

*/
int	draw_path(t_bfs *bfs)
{
	int	stop_x;
	int	stop_y;
	int	record;

	bfs->x = bfs->p_x;
	bfs->y = bfs->p_y;
	stop_x = bfs->en_x;
	stop_y = bfs->en_y;
	bfs->position[bfs->x][bfs->y].distance[bfs->n] = bfs->mark;
	if (find_shortest_route(bfs, bfs->x, bfs->y) == -1)
	{
		return (normalize_path(bfs, bfs->map_x, bfs->map_y), false);
	}
	while ((bfs->x != stop_x) || (bfs->y != stop_y))
	{
		if (found_end(bfs->x, bfs->y, stop_x, stop_y) == true)
		{
			bfs->position[bfs->en_x][bfs->en_y].distance[bfs->n] = bfs->mark;
			break ;
		}
		record = find_shortest_route(bfs, bfs->x, bfs->y);
		update_one(bfs, &bfs->x, &bfs->y, record);
	}
	return (normalize_path(bfs, bfs->map_x, bfs->map_y), true);
}
