
#ifndef LIB_H
# define LIB_H

# define KEYLEN 16
# define OPT3 "3 -> Decripta stringa di testo"
# define KEYOK1 "| CHIAVE DI CRIPTAZIONE CREATA CON SUCCESSO |"
# define KEYOK2 " CHIAVE DI CRIPTAZIONE CREATA CON SUCCESSO "
# define NEWOPT "| Vuoi eseguire altre operazioni? (Y/N) |"

// self made header files
# include "colors.h"
# include "../get_next_line/get_next_line.h"

// standard header files
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <time.h>
# include <sys/stat.h>
# include <dirent.h>
# include <sys/types.h>
# include <fcntl.h>

// crypt_string.c
void	crypt_str(char *str);
void	key_generator(char *str);

// crypt_file.c
void	crypt_file(void);

// decrypt_string.c
void	decrypt_str(char *str);

// decrypt_file.c
void	decrypt_file(void);

// utils.c
char			*ft_substr(char const *s, unsigned int start, size_t len);
char			*find_filename(char *path);
void			ft_copystr(const char s1[1024], char *s2); //done only for making run filter function in crypt_file.c
unsigned char	*crypt_name(char *str, char *key);
unsigned char 	*ft_unstrjoin(unsigned char *s1, unsigned char *s2);
int				find_newline(char *str);
void			print_format(char c, int len, int newline);
int 			ft_strlenfinal(char *str);
char			*ft_strappend(char *s1, char *s2);

#endif