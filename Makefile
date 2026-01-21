NAME = webserv

CC = c++
FLAGS = -Wall -Wextra -Werror -g -D_GLIBCXX_DEBUG
CPPFLAGS = -std=c++98


SRCS = $(addprefix srcs/, main.cpp $(CPP) $(PARSING) $(UTILS))

PARSING = $(addprefix parsing/, parseConf.cpp parseRequest.cpp $(CONF_PARSING))
CONF_PARSING = $(addprefix conf/, parseEvent.cpp parseMain.cpp parseServer.cpp parseLocation.cpp parseHttp.cpp)

CPP = $(addprefix cpp/, Server.cpp Client.cpp Request.cpp \
		$(addprefix conf/, getter.cpp setter.cpp print.cpp Conf.cpp))
UTILS = $(addprefix utils/, utils_page1.cpp path_checker.cpp value_checker.cpp \
		string/string.cpp)

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

run+: re
	clear ; echo "Grazie per aver scelto il servizio plus!\nOra potrete usufruire del servizio pulizia piedi della zia del tuo ragazzo senza costi aggiuntivi!" && ./$(NAME)

spaghetti: re
	clear ; ./$(NAME)

gdb: re
	clear ; gdb ./$(NAME)

giddibi: re
	clear ; gdb ./$(NAME)

val: $(NAME)
	clear ; valgrind ./$(NAME)

.PHONY: all clean fclean run val
