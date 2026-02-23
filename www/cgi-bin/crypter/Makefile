# Nome dell'eseguibile finale
TARGET = crypter.cgi

# Compilatore da utilizzare boh usiamo gcc
CC = cc

# Flag di compilazione, qui sono indicate le flag di compilazione e l'inclusione della directory con il file header.h (il "-I" prima del nome della directory in cui è incluso il file .header gli dice di includerlo)
CFLAGS = -Wall -Werror -Wextra -Iincludes -g

# File sorgenti, questi sono i file sorgenti in .c che poi verranno compilati in oggetti
SRCS = main.c utils.c crypt_string.c decrypt_string.c crypt_file.c decrypt_file.c

GNL = $(addprefix get_next_line/, get_next_line.c get_next_line_utils.c)

# File oggetto (i file oggetto sono il risultato della compilazione dei file .c e verranno utilizzati da make per creare il file di output)
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

# Regola principale (default) per costruire il programma, (TARGET) sta ad indicare il nome che verrà dato all'eseguibile finale e (OBJS) gli oggetti prodotti dalla compilazione dei file sorgente 
$(TARGET): $(GNL) $(SRCS) 
	$(CC) $(CFLAGS) $^ -o $(TARGET)

# Regola per pulire i file generati
clean:
	rm -f $(OBJS)

fclean: clean
	rm -rf $(TARGET)

re: fclean all
