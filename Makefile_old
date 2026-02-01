NAME = webserv

CC = c++
FLAGS = -Wall -Wextra -Werror -g -D_GLIBCXX_DEBUG
CPPFLAGS = -std=c++98


SRCS = $(addprefix srcs/, main.cpp $(CPP) $(PARSING) $(UTILS))

PARSING = $(addprefix parsing/, parseConf.cpp parseRequest.cpp $(CONF_PARSING) $(REQUEST_PARSING))
CONF_PARSING = $(addprefix conf/, parseEvent.cpp parseMain.cpp parseServer.cpp parseLocation.cpp parseHttp.cpp)
REQUEST_PARSING = $(addprefix request/, utils.cpp)

CPP = $(addprefix cpp/, 		Client.cpp \
		$(addprefix Server/,	Server.cpp getter.cpp setter.cpp methods.cpp setup_utils.cpp)\
		$(addprefix conf/, 		getter.cpp setter.cpp print.cpp Conf.cpp) \
		$(addprefix request/, 	getter.cpp setter.cpp print.cpp Request.cpp))
UTILS = $(addprefix utils/, 	utils_page1.cpp path_checker.cpp value_checker.cpp \
								string/string.cpp url.cpp env.cpp directory.cpp \
								cgi.cpp html_pokedex.cpp)

all: $(NAME)

$(NAME): $(SRCS)
	$(CC) $(FLAGS) $(CPPFLAGS) $^ -o $(NAME)

clean:
	@echo "wassup"

fclean:
	rm -f $(NAME)

re: fclean all

runner: $(NAME)
	clear ; ./$(NAME)

# BladeRunner: 
# 	xgd open https://www.youtube.com/results?search_query=bayblade+sigla
run:
	clear ; ./$(NAME)

spaghetti: re
	clear ; ./$(NAME)

gdb: re
	clear ; gdb ./$(NAME)

giddibi: gdb

val: $(NAME)
	clear ; valgrind ./$(NAME)

.PHONY: all clean fclean run val
