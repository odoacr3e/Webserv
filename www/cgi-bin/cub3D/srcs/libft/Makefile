NAME = libft.a
GNL_PATH = ../getnextline


SRC = 	$(wildcard *.c) \
      $(wildcard list/*.c) \
      $(wildcard map/*.c) \
		$(GNL_PATH)/get_next_line.c	 $(GNL_PATH)/get_next_line_utils.c

OBJ = $(SRC:.c=.o)

BONUS = ft_lstnew.c ft_lstadd_front.c ft_lstsize.c ft_lstlast.c ft_lstadd_back.c \
		ft_lstdelone.c ft_lstclear.c ft_lstiter.c ft_lstmap.c

BONUSOBJ = $(BONUS:.c=.o)

CC = cc

CFLAGS = -Wall -Werror -Wextra -g

RM = rm -rf

all: $(NAME)

$(NAME): $(OBJ)
	ar rcs $(NAME) $(OBJ)

clean:
	$(RM) $(OBJ) $(BONUSOBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all 

bonus: $(OBJ) $(BONUSOBJ)
	ar rcs $(NAME) $(OBJ) $(BONUSOBJ)

.PHONY: all clean fclean re bonus