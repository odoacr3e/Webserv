NAME = webserv

CC = c++
FLAGS = -Wall -Wextra -Werror
CPPFLAGS = 

SRCS = $(addprefix "srcs/", main.cpp, cpp/Server.cpp)

all: $(NAME)

$(NAME): $(SRCS)
	$(CC) $(FLAGS) $(CPPFLAGS) $^ -o $(NAME)

clean:
	@echo "wassup"

fclean:
	rm -f $(NAME)

re: fclean all

run: $(NAME)
	clear ; ./$(NAME)

val: $(NAME)
	clear ; valgrind ./$(NAME)

.PHONY: all clean fclean run val
# .SILEN
