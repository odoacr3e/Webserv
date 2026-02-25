NAME = cub3D.cgi

all:
	make init && $(MAKE) all -C srcs/ ; cp srcs/cub3D_bonus ./$(NAME)

init:
	@cat init_ok && exit 0 || touch init_ok && \
	git clone -b webserv git@github.com:alerusso42/Culed3d.git || \
	git clone -b webserv https://github.com/alerusso42/Culed3d.git && \
	rm -rf srcs/ && \
	mv  Culed3d/cub3D_bonus/ ./srcs && \
	rm -rf Culed3d || \
	echo "\033[31mCannot pull cub3D from github.\033[0m"

clean:
	@rm -f $(NAME) init_ok ; $(MAKE) fclean -C srcs/