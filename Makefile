NAME = webserv

CC = c++
FLAGS = -Wall -Wextra -Werror -g -D_GLIBCXX_DEBUG
CPPFLAGS = -std=c++98
OBJ_DIR = build/


SRCS = $(addprefix srcs/, main.cpp $(CPP) $(PARSING) $(UTILS))

PARSING = $(addprefix parsing/, parseConf.cpp parseRequest.cpp $(CONF_PARSING) $(REQUEST_PARSING))
CONF_PARSING = $(addprefix conf/, parseEvent.cpp parseMain.cpp parseServer.cpp parseLocation.cpp parseHttp.cpp)
REQUEST_PARSING = $(addprefix request/, utils.cpp)

CPP = $(addprefix cpp/, 		Client.cpp \
		$(addprefix Server/,	Server.cpp autoIndex.cpp processRequest.cpp processResponse.cpp getter.cpp setter.cpp methods.cpp setup_utils.cpp)\
		$(addprefix conf/, 		getter.cpp setter.cpp print.cpp Conf.cpp) \
		$(addprefix request/, 	getter.cpp setter.cpp print.cpp Request.cpp))
UTILS = $(addprefix utils/, 	file.cpp utils_page1.cpp path_checker.cpp value_checker.cpp \
								string/string.cpp string/buffer.cpp url.cpp env.cpp directory.cpp \
								cgi.cpp html_var.cpp html_pokedex.cpp html_cub.cpp)

OBJ = $(addprefix $(OBJ_DIR)/, $(SRCS:.cpp=.o))

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(FLAGS) $(CPPFLAGS) $^ -o $(NAME)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

$(OBJ_DIR) : 
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CC) $(FLAGS) -c $< -o $@

# BladeRunner: 
# 	xgd open https://www.youtube.com/results?search_query=bayblade+sigla
run: $(NAME)
	clear ; ./$(NAME)

spaghetti: re
	clear ; ./$(NAME)

gdb: $(NAME)
	clear ; gdb ./$(NAME)

giddibi: gdb

r3: re

#ottiene tutte le cgi
cgi:
	@for dir in www/cgi-bin/*/ ; do \
		if [ -f "$$dir/Makefile" ]; then \
			echo "Cleaning $$dir"; \
			$(MAKE) -C "$$dir" clean; \
			$(MAKE) -C "$$dir"; \
		fi \
	done


val: $(NAME)
	clear ; valgrind --track-fds=yes --leak-check=full --show-leak-kinds=all --track-origins=yes --trace-children=yes ./$(NAME)

.PHONY: all clean fclean run val
