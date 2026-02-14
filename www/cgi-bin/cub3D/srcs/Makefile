# Top level target
NAME     = cub3D_bonus
SRC_PATH = srcs/

# Compiler settings (lm == math.h)
CC       = cc
CFLAGS   = -Wall -Werror -Wextra -g
#LFLAGS   =  -I./libft -Lminilibx-linux -lmlx -lX11 -lm -lXext
LFLAGS   =  -I./libft -lm
DEBUG    = -D DEBUG=true
NORMAL   = -D DEBUG=false

# Library 
LIBFT_DIR = libft
LIBFT     = $(LIBFT_DIR)/libft.a
PARS_DIR  = parsing

# All source files, with their relative paths c
SRCS = $(addprefix $(SRC_PATH), \
	main_bonus.c \
	init/mem_handler_bonus.c \
	init/entity_bonus.c \
	init/get_texture_bonus.c \
	init/texture_list_bonus.c \
	input/commands_bonus.c \
	input/commands2_bonus.c \
	input/interact_bonus.c \
	input/move_bonus.c \
	parsing/parsing_bonus.c \
	parsing/get_type_bonus.c \
	parsing/get_map_bonus.c \
	parsing/check_textures_bonus.c \
	parsing/check_map_bonus.c \
	parsing/check_map_access_bonus.c \
	utils/animation.c \
	utils/bitwise_bonus.c \
	utils/debug_bonus.c \
	utils/strings_bonus.c \
	utils/entity_bonus.c \
	utils/error_bonus.c \
	utils/time_bonus.c \
	utils/math_bonus.c \
	utils/minimap_bonus.c \
	utils/parsing_bonus.c \
	utils/ray_utils_bonus.c \
	utils/alloc_utils_bonus.c \
	utils/render_bonus.c \
	utils/render2_bonus.c \
	utils/render3_bonus.c \
	utils/texture_filters.c \
	utils/texture_select.c \
	utils/xpm_bonus.c \
	render/battle_bonus.c \
	render/game_loop_bonus.c \
	render/raycast_bonus.c \
	render/entity_bonus.c \
	render/menu_bonus.c \
	sfx/sfx_bonus.c \
)
all: $(NAME)

$(NAME): $(LIBFT) $(SRCS) cub3D_bonus.h files_bonus.h
	$(CC) $(CFLAGS) $(SRCS) $(LIBFT) $(LFLAGS) -o $(NAME)

$(LIBFT): 
	$(MAKE) all -C $(LIBFT_DIR)

gdb: $(NAME)
	gdb -x ../a.gdb --args ./cub3D_bonus maps/debug.cub

gdbtui: $(NAME)
	gdb --tui -x a.gdb --args ./cub3D_bonus maps/debug.cub

clean:
	rm -rf $(OBJ_DIR)
	$(MAKE) clean -C $(LIBFT_DIR)

fclean: clean
	rm -f $(NAME)
	$(MAKE) fclean -C $(LIBFT_DIR)
	rm -rf minilibx-linux *.out *.txt

bonus.c: fclean $(NAME)

re: fclean all

start: fclean all

pull: 
	(cd ../ ; git pull)

push: 
	(cd ../ ; ./upd.sh)

run:
	clear ; make && ./$(NAME) maps/$(MAP).cub

val: all
	valgrind --suppressions=../s.supp --leak-check=full --show-leak-kinds=all --track-origins=yes -s --quiet ./$(NAME) maps/$(MAP).cub

# Target speciale per il profiling
prof: CFLAGS += -pg
prof: LDFLAGS += -pg
prof: re
	./cub3D_bonus; gprof ./cub3D_bonus gmon.out > profiling_results.txt; make prof_read
	
prof_read:
	less profiling_results.txt

#

.PHONY: all clean fclean re libft
.SILENT: