/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3D_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alerusso <alessandro.russo.frc@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 17:19:17 by alerusso          #+#    #+#             */
/*   Updated: 2026/02/12 16:10:44 by alerusso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_BONUS_H
# define CUB3D_BONUS_H
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <bsd/string.h>
# include <ctype.h>
# include <limits.h>
# include <math.h>
# include <sys/time.h>
# include <sys/stat.h>
# include <signal.h>
# include <stdbool.h>
# include <math.h>
# include <stdlib.h>
# include "libft/libft.h"
# include "files_bonus.h"

# define RST		"\033[0m" /* Reset to default color */
# define RED		"\033[1;31m" /* Bold Red */
# define BGREEN		"\033[1;32m" /* Bold Green */
# define BYELLOW	"\033[1;33m" /* Bold Yellow*/
# define BBLUE		"\033[1;34m" /* Bold Blue*/
# define BMAGENTA	"\033[1;35m" /* Bold Magenta*/
# define BCYAN		"\033[1;36m" /* Bold Cyan*/
# define BWHITE		"\033[1;37m" /* Bold White*/

# define FPS 60
# define FRAME_TIME 16666

# define DEBUG false
# define VALID_CHARS " 01NSEWDFC"
# define PLAYER_CHARS "NSEWP"
# define FFILL_CHARS "0NSEW"
# define WALL_CHARS "1 "

# define HIMG 64
# define WIMG 64
# define WIMG_MINIMAP 24
# define HIMG_MINIMAP 24
# define MINIMAP 15
# define TXTR 300
# define PIX_PLAYER 96
# define PLAYER_OFFSET 32
# define SHADE_INTENSITY 60
# define WSCREEN 1920
# define HSCREEN 1200
# define HALF_SCREEN_X 960
# define HALF_SCREEN_Y 600
# define ENTITY_WIDTH 42

//# define RADIANT 0.008726

/*
	180 degrees are PI radiant.
	Degree changes every PI / 180.
*/
# define RADIANT 0.017453292

//	Amount of radiant rotated for every rotation.
# define ANGULAR_SPEED 0.043633231

//	Amount of movement per frame.
# define PLAYER_SPEED 10000

//	Mouse rotation speed, per frame.
# define SENSITIVITY 0.2

//	FOV (=Field Of View) represents the angle of the player vision.
# define FOV 60

# define PI 3.1415926

//FIXME	This flag is in debug state.
# define SPEED 0.01
# define ANIMATION_SPEED 30

# define ANGLE_0 0
# define ANGLE_1 RADIANT
# define ANGLE_20 0.34906584
# define ANGLE_90 1.5707963
# define ANGLE_180 3.1415926
# define ANGLE_270 4.7123889
# define ANGLE_360 6.2831852

typedef struct timeval		t_time;

typedef struct s_texture
{
	void			*ptr;
	char			*path;
	unsigned char	*xpm;
	double			shade;
	double			scaler[2];
	int				size[2];
	int				i;
	int				bpp;
	int				cpp;
	int				endian;
	int				total_size;
	int				offset;
	int				colors;
	char			filters;
}	t_txtr;

typedef struct s_entity
{
	double	curr_x;
	double	curr_y;
	double	pov[2];
	double	speed;
	double	first_ray;
	double	last_ray;
	int		screen[2];
	int		map[2];
	int		contact_column;
	int		contact_num;
	int		distance;
	t_txtr	**frames;
	int		f_curr;
	int		f_time;
	int		f_elapsed;
	int		vite_rimaste;
	char	type;
	char	input;
	bool	contact;
}	t_entity;

typedef struct s_data
{
	char		events[64];
	t_entity	player;
	t_time		start;
	t_entity	**renderer;
	t_entity	*doors;
	t_entity	*enemies;
	t_entity	*items;
	t_entity	*foe;
	void		*mlx;
	void		*win;
	t_txtr		*txtr;
	char		**map;
	char		*txtr_floor;
	char		*txtr_ceiling;
	int			ent_num;
	int			floor[3];
	int			ceiling[3];
	int			max_x;
	int			max_y;
	int			column;
	int			audio_pid;
	int			enemy_audio;
	int			result;
	char		audio_play;
	char		button;
	char		enemy_choice;
	char		menu;
	bool		battle;
}	t_data;

typedef enum e_timecode
{
	SECONDS,
	MILLISECONDS,
	MICROSECONDS,
}	t_timecode;

enum	e_errors
{
	E_MALLOC = 1,
	E_OPEN,
	E_ARGC,
	E_EXT,
	E_TYPE,
	E_FORMAT,
	E_TEXTURE,
	E_COLORS,
	E_NO_MAP,
	E_CHAR,
	E_NO_PLAYER,
	E_MULTIPLAYER,
	E_INVALID_MAP,
	E_MLX_TEXTURE,
	E_INVALID_PATH,
	E_INVALID_DOOR,
	E_INVALID_ENEMY,
	E_INPUT,
};

enum	e_type_identifers
{
	NO = 0,
	SO = 2,
	EA = 4,
	WE = 8,
	F,
	C,
	TYPE_IDENTIFIERS_NUM = 6,
};

enum	e_texture_filters
{
	FILTER_ON = 255,
	FILTER_BLACK_WHITE = 1,
	FILTER_BUTTON_PRESSED = 2,
};

enum e_utils
{
	X = 0,
	Y = 1,
	COS = 0,
	SIN = 1,
	POSITIVE = 1,
	NEGATIVE = -1,
	LEFT = 1,
	RIGHT = 2,
	UP = 4,
	DOWN = 8,
	MOVEMENT = LEFT | RIGHT | UP | DOWN,
	R_LEFT = 16,
	R_RIGHT = 32,
	ROTATION = R_LEFT | R_RIGHT,
	BPP = 0,
	SIZE = 1,
	ENDIAN = 2,
	ENTITY_NOT_FOUND = -1,
	ENTITY_END = 0,
	ENTITY_DOOR = 1,
	ENTITY_ENEMY = 'F',
	ENTITY_ITEM = 'C',
	MOUSE_LEFT = 1,
	MOUSE_RIGHT = 3,
	MOUSE_MIDDLE = 2,
	MOUSE_UP = 4,
	MOUSE_DOWN = 5,
	PLAY = 1,
	EXIT = 2,
	CHOICE_ROCK = 1,
	CHOICE_PAPER = 2,
	CHOICE_SCISSOR = 3,
	CHOICES = 3,
	RESULT_WIN,
	RESULT_LOSS,
	RESULT_DRAW,
};

void	webserv_porting(t_data *data);

//void	fill_txtr(t_data *data, t_map *map, int index, int size[2]);
void	fill_txtr(t_data *data, t_map *map, int index);
void	txtr_list(t_data *data);
void	txtr_list2(t_data *data);
void	mini_player_txtr_list(t_data *data, int *size);
void	foe_txtr_list(t_data *data, int *size);
void	init_entity(t_data *data, t_entity **entity, int n, char c);
void	parsing(t_data *data, int argc, char **argv);
void	error(t_data *data, int err, char *file);
void	lets_start_the_party(t_data *data);
void	set_txtr_null(t_data *data);
void	spread_democracy(t_data *data);
void	free_texture(t_data *data);

//SECTION	input

void	move(t_data *data, t_entity *entity, double angle[]);
int		mouse_hook(int button, int x, int y, t_data *param);
void	battle_commands(t_data *data, int keycode);
void	rotate(t_data *data, t_entity *entity);
int		commands_release(int keycode, t_data *data);
int		commands_press(int keycode, t_data *data);
void	init_doors(t_data *data, int n_doors);
int		ft_cross_close(t_data *data);
void	mouse_input(t_data *data);
int		move_player(t_data *data);
void	interact(t_data *data);
void	init_player(t_data *data);

//SECTION	parsing

void	check_textures(t_data *data, int fd);
void	get_type(t_data *data, int fd);
void	check_map_access(t_data *data);
void	get_map(t_data *data, int fd);
void	check_other(t_data *data);
void	check_chars(t_data *data);
void	check_walls(t_data *data);
void	check_doors(t_data *data);
void	parse_map(t_data *data);
void	finish_him(int fd);

//SECTION	utils

int		collision_checker(t_entity *ray, t_data *data, double angle, char type);
t_txtr	*texture_finder(t_data *data, double ray_angle, int hit_x, int hit_y);
bool	collision_entity(t_data *data, int x, int y, bool discard_open_door);
void	init_line_data(t_data *data, t_entity *entity_data, double pov_x);
void	init_animation(t_data *data, t_entity *entity, int n, int first);
int		wall_height(t_data *data, double x, double y, double ray_angle);
void	parse_xpm(t_data *data, t_txtr *txtr, t_map *map, int name);
void	update_delta(double pov, double *delta_x, double *delta_y);
int		wall_face(t_data *data, t_entity *entity, double angle);
int		count_chars(t_data *data, int *count, char *search);
void	txtr_filters(t_txtr *txtr, int *r, int *g, int *b);
void	put_pixel(t_data *data, int x, int y, int color);
void	render_arms(t_data *data, t_entity *player);
void	*which_entity(t_data *data, int x, int y);
double	ray_lenght(t_data *data, int rx, int ry);
double	safe_division(double delta, double sum);
bool	value_changed(void *value, size_t type);
int		entity_type(t_data *data, int x, int y);
void	fill_array(int a, int b, int array[2]);
bool	aspettanding(int time_to_wait, int i);
int		get_pixel_color(t_txtr *txtr, int i);
void	update_all_animations(t_data *data);
void	update_coord(t_entity *entity_data);
void	update_animation(t_entity *entity);
int		bigger(int a, int b, int c, int d);
void	bitwise_xor(char *byte, char bit);
void	sort_render(t_entity **renderer);
void	ft_sleep(long long microsecond);
void	normalize_angle(double *angle);
void	clear_window(t_data *data);
long	elapsed_time(t_time start);
void	render_cross(t_data *data);
void	*safe_malloc(size_t size);
int		rgb(int r, int g, int b);
int		which_p(t_data *data);
int		get_rand(int max);
double	round_rad(double rad);
double	grad2rad(double rad);
double	rad2deg(double rad);
void	delete(void **ptr);

//SECTION debug

int		map_start(t_data *data, int i, int j);
int		game_loop(t_data *data);

//SECTION render

void	put_image_to_image(t_data *data, int which, int pos[2], int size[2]);
void	put_image_resize(t_data *data, int which, int pos[2], int size[2]);
void	line(t_data *data, t_entity *entity, double angle, char collision);
void	draw_wall(t_data *data, int pos[2], double ray_angle);
void	render_column(t_data *data, t_txtr *txtr, double h);
void	animation(t_data *data, t_entity *entity);
void	wall(t_data *data, double x, void *txtr);
int		compute_line(t_data *data, double pov_x);
void	init_matrix(void **matrix, int size);
int		commands(int key, t_data *data);
void	reset_renderer(t_data *data);
void	render_entity(t_data *data);
void	backgrounder(t_data *data);
void	main_menu(t_data *data);
void	get_txtr(t_data *data);
int		which_p(t_data *data);

void	update_map(t_data *data, t_entity *entity, int new_x, int new_y);
void	render_sync(t_data *data);
int		texture_x_offset(t_data *data, double ray_angle, int hit_x, int hit_y);
bool	all_collision(t_data *data, int x, int y);

void	play_audio(char *audio_path, t_data *data);
void	stop_audio(t_data *data);

void	battle(t_data *data);
void	hai_guardato(t_data *data);

#endif
