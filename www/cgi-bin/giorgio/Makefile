NAME = giorgio.cgi

COMP  = c++ -Wall -Wextra -Werror -g -D_GLIBCXX_DEBUG -std=c++98

all: $(NAME)


$(NAME) : youtube.cpp
	$(COMP) youtube.cpp -o $(NAME)

clean :
	rm -f $(NAME)