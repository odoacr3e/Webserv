/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entity_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alerusso <alerusso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 14:44:59 by alerusso          #+#    #+#             */
/*   Updated: 2025/08/28 09:43:18 by alerusso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cub3D_bonus.h"

static void		render_one(t_data *data, t_entity *entity);
static double	entity_scaler_x(t_entity *entity, t_txtr *txtr, double h);
static void		draw_one(t_data *data, t_txtr *txtr, double scaler_x, double h);

/*
	data->renderer is an array of texture.
	is filled in every frame with all the entities the player sees.
	render_entity just render this array.
*/
void	render_entity(t_data *data)
{
	int	i;
	int	audio_stop;

	audio_stop = true;
	sort_render(data->renderer);
	i = 0;
	while (data->renderer[i])
	{
		if (data->renderer[i]->distance < 150)
			audio_stop = false;
		if (data->renderer[i]->frames && data->renderer[i]->contact_num)
			render_one(data, data->renderer[i]);
		++i;
	}
	if (audio_stop && data->enemy_audio)
	{
		data->enemy_audio = false;
		play_audio(SFX_GAME, data);
	}
}

/*
	1)	txtr is taken from the current entity frame;
	2)	entity height is calculated using the angle of the first ray that
		saw it;
	3)	we calculate the scaler in the x axis, depending on entity height
		(SCALER_X = how many column skip/repeat from the xpm file);
	4)	we calculate the shade of the texture. SHADE_INTENSITY is divided,
		to make entities brighter than walls;
	5)	in draw_one, the entire entity is drawn;
	6)	entity rays data are resetted.
*/
static void	render_one(t_data *data, t_entity *entity)
{
	double	scaler_x;
	t_txtr	*txtr;
	double	h;

	txtr = entity->frames[entity->f_curr];
	h = wall_height(data, entity->screen[X], entity->screen[Y], \
entity->first_ray);
	h *= 2.5;
	if (h < 20)
		h = 20;
	scaler_x = entity_scaler_x(entity, txtr, h);
	data->column = entity->contact_column;
	txtr->shade = h / (SHADE_INTENSITY / 3);
	if (txtr->shade > 1)
		txtr->shade = 1;
	draw_one(data, txtr, scaler_x, h);
	entity->contact_num = 0;
	entity->contact = false;
	txtr->offset = 0;
}

/*
//	offset is the current column that is being printed in the xpm.
	is a double variable, to be able to scale correctly with the
	scaler_x.

//	txtr->offset represent the same value, but is slightly different.
	all columns value, for how xpm array is done, must
	be multiple of "txtr->bpp / 8" (BPP = Byte Per Pixel).
	so, for example, an xpm file with bpp = 4 looks like this:
	[0]	[1]	[2]	[3]	[4]	[5]	[6]	[7]	[8]	[9]	[10]	[11]	[12]	...
	[R]	[G]	[B]	[A]	[R]	[G]	[B]	[A]	[R]	[G]	[B]		[A]		[R]		...

	R = red;	G = green;	B = blue;	A = alpha (transparency).
	Therefore, we need to access in the array every 4 bytes, to get regular
	RGBA values.
	the offset variable just increase itself by scaler_x, so if scaler_x = 2,
	will skip two pixels, but this is wrong because we would get RGBA values,
	and then a BARG value;
	so, we use it to scale properly, but then we use txtr->offset, that is 
	rounded by "txtr->bpp / 8": so, we'll always have RGBA values.
	double	offset	------------>	(0:R;2:B;4:R;6:B;8:R;...)
	int		txtr->offset	---->	(0:R;0:R;4:R;4:R;8:R;...)
	
	1)	we loop until all columms are covered;
	2)	we render the current column;
	3)	offset is increase by scaler_x;
	4)	txtr->offset is first set by offset, then rounded by "txtr->bpp / 8".
*/
static void	draw_one(t_data *data, t_txtr *txtr, double scaler_x, double h)
{
	double	offset;

	txtr->offset = 0;
	offset = 0;
	while (txtr->offset < txtr->size[X])
	{
		render_column(data, txtr, h);
		++data->column;
		offset += scaler_x;
		txtr->offset = (int)offset;
		while (txtr->offset % (txtr->bpp / 8))
			--txtr->offset;
	}
}

/*
	scaler:	it says how many columns of pixels should be repeated/skipped in the
			xpm to make the image looks scaled to the width on the screen;
	wide:	it is used to make the scaler_x proportional to the height of the
			rendered image on the screen.
	
	1)	the height is divided by a macro constant;
	2)	the scaling is done by dividing the maximum possible width on screen,
		that is WSCREEN, for the number of rays that has touched the entity.
		this works because the number of rays is literally WSCREEN;
	3)	scaler is then reduced dividing it by the wide variable.
		Otherwise it wouldn't be proportional to the height;
	4)	scaler is then multipled by the Byte Per Pixel of the image.
		Actually, this is almost always equal to multiply by 4.
		Just a way to ignore the number of pixels color in the size of
		the texture;
	5)	finally, the original texture width is considered.
		txtr->scaler[X] is: SIZE_OF_THE_IMAGE / TXTR
		SIZE_OF_THE_IMAGE:	the number of columns in the xpm;
		TXTR:				the lenght of the textures.
		In this way, all images are normalized by TXTR macro;
	6)	after this steps, the scaler is returned back to render_one.
*/
static double	entity_scaler_x(t_entity *entity, t_txtr *txtr, double h)
{
	double	scaler;
	double	wide;

	scaler = 0;
	wide = h / ENTITY_WIDTH;
	if (wide < 1)
		wide = 1;
	else if (wide > 4)
		wide = 4;
	scaler = (WSCREEN / (double)entity->contact_num);
	scaler /= wide;
	scaler = scaler * (txtr->bpp / 8);
	scaler *= txtr->scaler[X];
	return (scaler);
}

void	reset_renderer(t_data *data)
{
	int	i;

	i = -1;
	while (data->renderer[++i])
		data->renderer[i] = NULL;
}
