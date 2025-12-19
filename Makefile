NAME = webserv

CC = c++
FLAGS = -Wall -Wextra -Werror -g -D_GLIBCXX_DEBUG
CPPFLAGS = -std=c++98


SRCS = $(addprefix srcs/, main.cpp $(CPP) $(PARSING) $(UTILS))

PARSING = $(addprefix parsing/, parseConf.cpp parseRequest.cpp $(CONF_PARSING))
CONF_PARSING = $(addprefix conf/, parseEvent.cpp parseMain.cpp parseServer.cpp parseLocation.cpp parseHttp.cpp)

CPP = $(addprefix cpp/, Server.cpp Client.cpp Request.cpp Conf.cpp)
UTILS = $(addprefix utils/, utils_page1.cpp path_checker.cpp value_checker.cpp)

all: $(NAME)

$(NAME): $(SRCS)
	$(CC) $(FLAGS) $(CPPFLAGS) $^ -o $(NAME)

clean:
	@echo "wassup"

fclean:
	rm -f $(NAME)

re: fclean all

run: re
	clear ; ./$(NAME)

gdb: re
	clear ; gdb ./$(NAME)

val: $(NAME)
	clear ; valgrind ./$(NAME)

.PHONY: all clean fclean run val
