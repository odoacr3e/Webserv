CGIS = pokemon moves

CC      = cc
CFLAGS  = -Wall -Werror -Wextra -g
SRC_DIR = srcs
OUT_DIR = output

all: $(CGIS:%=$(OUT_DIR)/%.cgi)

$(OUT_DIR)/%.cgi: $(SRC_DIR)/%.c libft.a
	$(CC) $(CFLAGS) $< libft.a -o $@

clean:
	rm -f output/*