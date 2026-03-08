/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   all.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alerusso <alerusso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 15:30:58 by alerusso          #+#    #+#             */
/*   Updated: 2026/01/27 15:02:52 by alerusso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ALL_H
# define ALL_H
# ifndef FUN_MACROS
#  define FUN_MACROS
#  define MALLOC malloc
#  define CALLOC ft_calloc
#  define FREE free
#  define WRITE writefd
#  define READ readfd
#  define SEEK lseek
#  define OPEN ft_open
#  define CLOSE close
#  define SDL_RWops int
# endif
# include <stdint.h>
# include <stdlib.h>
void	*free_matrix(char **matrix);
void	*free_three_d_matrix(char ***matrix);

// Memoria statica usata ---> (BUFFER_SIZE + 1) * ((MAX_FILES + 1) * 20)
#ifndef MFILE_H
# define MFILE_H
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1000
# endif
# ifndef MAX_FILES
#  define MAX_FILES 20
# endif
# include <unistd.h>
# include <string.h>
# include <stdarg.h>
# include <stdlib.h>
# include <stdbool.h>
# include <stdio.h>
# include <fcntl.h>
# include <limits.h>

typedef struct s_fd
{
	int			p;
	int			n;
}	t_fd;

typedef struct s_manage_fds
{
	char	buffer[MAX_FILES + 1][BUFFER_SIZE + 1];
	t_fd	fds[MAX_FILES + 1];
	char	*filenames[MAX_FILES + 1];
	t_fd	curr_fd;
	char	*curr_file;
	int		last;
}			t_manage_fds;

//		SDL_SUPPORT

t_fd			openfd(const char *filename, const char *permissions);
int				ft_open(const char *filename, const char *perm);
t_manage_fds	*fd_database(bool);
int				get_filedata(t_fd *fd, char **filename);
void			del_filedata(void);
int				switch_filedata(t_fd fd);
int				fd_indexation(void);
void			closefd(t_fd fd);
int				read_curr(t_manage_fds *data, int count);
int				readfd(t_fd	fd, char *buff, int count);
char			*get_static_buffer(int fd, bool reset, bool reset_all);
char			*gnl();
int				writefd(t_fd fd, const char *s, size_t len);
int				reset_fd(t_fd fd);

#endif
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   daft.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alerusso <alessandro.russo.frc@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 11:27:31 by alerusso          #+#    #+#             */
/*   Updated: 2026/01/02 12:46:50 by alerusso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DAFT_H
# define DAFT_H

/*WARNING: 	don't modify this file directly!
			modify SETTINGS.md instead!
*/

//SECTION - functions prototypes

int 	daft_init(const char *path, const char *settings_filename);
void	daft_quit(void);
void	daft_swap(int enum_id);
bool	daft_iter(void **ptr);
void	daft_free(void *mem);
void	*daft_get(const char *search);
int		daft_load(void);
void	*daft_edit(const char *search, int size, int mtr_number);
void	*daft_append(const char *key, int size, int mtr_number);

//SECTION - enum

typedef enum e_daft_db
{
	MOVES,
	POKEDEX,
}	t_daft_db;

#endif

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alerusso <alerusso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 19:34:47 by alerusso          #+#    #+#             */
/*   Updated: 2025/11/26 18:00:48 by alerusso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIST_H
# define LIST_H


typedef struct s_list	t_list;

typedef struct s_list
{
	t_list	*next;
	t_list	*prev;
	void	*content;
}				t_list;

typedef struct s_list2
{
	void	*content1;
	void	*content2;
	void	*next;
}				t_list2;

#endif
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 16:16:24 by codespace         #+#    #+#             */
/*   Updated: 2025/11/27 17:06:12 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

typedef struct s_fd 	t_fd;
typedef struct s_str 	t_str;

void	ft_putchar_fd(char c, t_fd fd);
void	ft_putendl_fd(char *s, t_fd fd);
void	ft_putnbr_fd(int n, t_fd fd);
void	ft_putstr_fd(char *s, t_fd fd);
int     tfd_printf(t_fd fd, const char *str, ...);
int     fd_printf(int fd, const char *str, ...);
int     ft_printf(const char *str, ...);
int     err_printf(const char *str, ...);
int     str_printf(t_str *buff, const char *str, ...);

#endif
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alerusso <alerusso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 20:24:29 by alerusso          #+#    #+#             */
/*   Updated: 2025/11/26 17:59:33 by alerusso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRING_H
# define STRING_H
# include <stdbool.h>
# include <unistd.h>
# include <limits.h>
# include <stdint.h>

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_macros.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alerusso <alessandro.russo.frc@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 12:50:27 by alerusso          #+#    #+#             */
/*   Updated: 2026/01/02 12:11:53 by alerusso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRING_MACROS_H
# define STRING_MACROS_H

typedef struct s_str			t_str;
typedef struct s_str_methods	t_str_methods;
typedef enum e_str_error		t_str_error;
typedef int32_t					err;

# define _STR_IDENTIFIER ULLONG_MAX
# define _STR_REALLOC_SIZE 32

# define STR_DEBUG false
# if STR_DEBUG == false
#  define STR_LOG(error, ...) ft_printf(2, error, ##__VA_ARGS__)
# else 
# define STR_LOG(error, ...) (void)0
# endif
//REVIEW - STR_OVERLOAD
/*
	f:	 	the name of the function
	name:	the name of the string object
	T:		the type of the variable it receives

	example: #define func_name(param)	STR_OVERLOAD(func_name, obj_name, param)
	see it as a switch case:

	switch (T)
	{
		case (char*):
			f ## _char	---> example:	print_char
		case (t_str):
			f ## _str	---> example:	print_str
		case (t_str*):
			f ## _ptr	---> example:	print_ptr
	}
*/
# define STR_OVERLOAD_CHECK(f, name, T) _Generic((T), \
		char*		:	f ## _char,\
		const char*	:	f ## _char,\
		void*		:	f ## _this,\
		t_str*		:	f ## _str,\
		const t_str*:	f ## _str)(name, T)

//SECTION - destructor

//	the function given to cleanup is called when the variable exits from its
//	scope
# define clean(base) __attribute__((cleanup(_ ## base ## _destructor)))
# define clean_ptr(base) __attribute__((cleanup(_ ## base ## _ptr_destructor)))

//SECTION - constructor

//	to call the constructor:	STR(variable_name, "initial buffer"), OR:
//								t_str	*s;
//								str_new(&s, "initial buffer")
//
//	in the first case, the object t_str is allocated on the stack (t_str);
//	in the second case, is allocated on the heap (t_str *).
//
//	first should be used as: 	string.print(&string);
//	second should be used as: 	string->print(string);
# define STR(name, init) t_str clean(str) name = _str_constructor(&name, init)

# ifndef SYSCALLS
#  define SYSCALLS
#  define FREE 		free
#  define MALLOC	malloc
#  define CALLOC	ft_calloc
# endif

# ifndef EXIT_SUCCESS
#  define EXIT_SUCCESS 0
# endif

#endif

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_methods.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alerusso <alessandro.russo.frc@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 13:23:44 by alerusso          #+#    #+#             */
/*   Updated: 2026/01/02 11:59:54 by alerusso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRING_METHODS_H
# define STRING_METHODS_H

typedef struct s_str			t_str;
typedef struct s_str_methods	t_str_methods;
typedef enum e_str_error		t_str_error;
typedef int32_t					err;

//public:

bool	str_new(t_str **str, const char *buff);
t_str	*str_delete(t_str *this);
void	str_terminate(void);
t_str	*str_set_start_index(t_str *str, int32_t i);

//private:

t_str	_str_constructor(t_str *str, const char *init);
void	_str_destructor(void *str);
err		_str_get_methods(t_str *s, bool reset);
t_str	*_str_set_error(t_str *str, int err, char *func_name);
void	_str_set(t_str *this);
t_str	*_str_reset(t_str *this, int i);
bool	_str_identifier(const void *p);

# define str_check(name, other)	STR_OVERLOAD_CHECK(str_check, name, other)
bool	str_check_char(t_str *this, const char *other);
bool	str_check_str(t_str *this, const t_str *other);
bool	str_check_this(t_str *this, const void *empty);

# define str_m(name, ...)	t_str	*(*name)(t_str *, __VA_ARGS__);

# define STR_METHODS	\
	int32_t	(*get_len)(t_str *);\
	void	(*set_i)(t_str *, int32_t);\
	int32_t	(*get_i)(t_str *);\
	str_m(str_addl, char c)\
	str_m(str_addr, char c)\
	str_m(str_app, const void *)\
	int32_t	(*str_cmp)(t_str *, const void *);\
	str_m(str_cpy, const void *)\
	str_m(str_cut, int32_t, int32_t)\
	str_m(str_diff, const void *)\
	str_m(str_excpy, const void *, const void *)\
	str_m(str_find, const void *)\
	str_m(str_first, const void *)\
	str_m(str_incpy, const void *, const void *)\
	str_m(str_itoa, int32_t)\
	str_m(str_join, const void *, int32_t n)\
	str_m(str_last, const void *)\
	t_str	*(*str_lower)(t_str *);\
	str_m(str_push, const void *)\
	int32_t	(*str_ncmp)(t_str *, const void *, int32_t);\
	str_m(str_ncpy, const void *, int32_t, int32_t)\
	t_str	*(*str_reverse)(t_str *);\
	str_m(str_rdiff, const void *)\
	str_m(str_rfind, const void *)\
	str_m(str_sdup, const void *)\
	err		(*str_satoi)(t_str *, int32_t *);\
	t_str	*(*str_sort)(t_str *);\
	t_str	*(*str_sfree)(t_str *);\
	str_m(str_srealloc, int32_t)\
	str_m(str_trim, int32_t)\
	t_str	*(*str_upper)(t_str *);

# define addl(name, ...)	str_addl(name, __VA_ARGS__)
t_str	*str_addl(t_str *this, char c);

# define addr(name, ...)	str_addr(name, __VA_ARGS__)
t_str	*str_addr(t_str *this, char c);

# define app(name, ...)		str_app(name, __VA_ARGS__)
t_str	*str_app(t_str *this, const void *other);
t_str 	*str_app_str(t_str *s, const t_str *s2);
t_str 	*str_app_char(t_str *s, const char *s2);

#define cmp(name, ...)		str_cmp(name, __VA_ARGS__)
int32_t	str_cmp(t_str *this, const void *other);
int32_t	str_cmp_char(t_str *this, const char *other);
int32_t	str_cmp_str(t_str *this, const t_str *other);

# define cpy(name, ...)		str_cpy(name, __VA_ARGS__)
t_str	*str_cpy(t_str *this, const void *other);
t_str	*str_cpy_char(t_str *this, const char *other);
t_str	*str_cpy_str(t_str *this, const t_str *other);

#define cut(name, ...)	str_cut(name, __VA_ARGS__)
t_str	*str_cut(t_str *this, int32_t start, int32_t end);

# define diff(name, ...)		str_diff(name, __VA_ARGS__)
t_str	*str_diff(t_str *this, const void *other);
t_str	*str_diff_chr(t_str *this, const char other);
t_str	*str_diff_char(t_str *this, const char *other);
t_str	*str_diff_str(t_str *this, const t_str *other);

# define excpy(name, ...)	str_excpy(name, __VA_ARGS__)
t_str	*str_excpy(t_str *this, const void *other, const void *set);

# define find(name, ...)		str_find(name, __VA_ARGS__)
t_str	*str_find(t_str *this, const void *other);
t_str	*str_find_chr(t_str *this, char other);
t_str	*str_find_char(t_str *this, const char *other);
t_str	*str_find_str(t_str *this, const t_str *other);

# define first(name, ...)		str_first(name, __VA_ARGS__)
t_str	*str_first(t_str *this, const void *other);
t_str	*str_first_chr(t_str *this, char other);
t_str	*str_first_char(t_str *this, const char *other);
t_str	*str_first_str(t_str *this, const t_str *other);

# define incpy(name, ...)	str_incpy(name, __VA_ARGS__)
t_str	*str_incpy(t_str *this, const void *other, const void *set);
t_str	*str_scpy_char(t_str *this, const char *other, const char *set, int m);
t_str	*str_scpy_str(t_str *this, const t_str *other, const char *set, int m);

#define itoa(name, ...)			str_itoa(name, __VA_ARGS__)
t_str	*str_itoa(t_str *this, int32_t value);

#define join(name, ...)		str_join(name, __VA_ARGS__)
t_str	*str_join(t_str *this, const void *other, int32_t n);
t_str	*str_join_char(t_str *this, const char *other, int32_t n);
t_str	*str_join_str(t_str *this, const t_str *other, int32_t n);

# define last(name, ...)		str_last(name, __VA_ARGS__)
t_str	*str_last(t_str *this, const void *other);
t_str	*str_last_chr(t_str *this, char other);
t_str	*str_last_char(t_str *this, const char *other);
t_str	*str_last_str(t_str *this, const t_str *other);

# define lower(name)			str_lower(name)
t_str	*str_lower(t_str *str);

#define ncmp(name, ...)		str_ncmp(name, __VA_ARGS__)
int32_t	str_ncmp(t_str *this, const void *other, int32_t n);
int32_t	str_ncmp_char(t_str *this, const char *other, int32_t n);
int32_t	str_ncmp_str(t_str *this, const t_str *other, int32_t n);

# define ncpy(name, ...)	str_ncpy(name, __VA_ARGS__)
t_str	*str_ncpy(t_str *this, const void *other, int32_t start, int32_t n);
t_str	*str_ncpy_char(t_str *this, const char *other, int32_t strt, int32_t n);
t_str	*str_ncpy_str(t_str *this, const t_str *other, int32_t strt, int32_t n);

# define push(name, ...)		str_push(name, __VA_ARGS__)
t_str	*str_push(t_str *this, const void *other);
t_str 	*str_push_str(t_str *s, const t_str *s2);
t_str 	*str_push_char(t_str *s, const char *s2);

# define rdiff(name, ...)		str_rdiff(name, __VA_ARGS__)
t_str	*str_rdiff(t_str *this, const void *other);
t_str	*str_rdiff_chr(t_str *this, char other);
t_str	*str_rdiff_char(t_str *this, const char *other);
t_str	*str_rdiff_str(t_str *this, const t_str *other);

# define reverse(name)			str_reverse(name)
t_str	*str_reverse(t_str *str);

# define rfind(name, ...)		str_rfind(name, __VA_ARGS__)
t_str	*str_rfind(t_str *this, const void *other);
t_str	*str_rfind_chr(t_str *this, char other);
t_str	*str_rfind_char(t_str *this, const char *other);
t_str	*str_rfind_str(t_str *this, const t_str *other);

# define satoi(name, T)	str_satoi(name, T)
err		str_satoi(t_str *str, int *n);

# define sdup(name, ...)		str_sdup(name, __VA_ARGS__)
t_str	*str_sdup(t_str *this, const void *other);
t_str	*str_sdup_char(t_str *this, const char *other);
t_str	*str_sdup_str(t_str *this, const t_str *other);

# define sort(name)				str_sort(name)
t_str	*str_sort(t_str *str);

# define sfree(name)	str_sfree(name)
t_str	*str_sfree(t_str *this);

# define srealloc(name, ...)	str_srealloc(name, __VA_ARGS__)
t_str	*str_srealloc(t_str *this, int32_t n);

# define trim(name, ...)		str_trim(name, __VA_ARGS__)
t_str	*str_trim(t_str *this, int32_t n);

# define upper(name)			str_upper(name)
t_str	*str_upper(t_str *str);

#endif



typedef struct s_str			t_str;
typedef struct s_str_methods	t_str_methods;
typedef enum e_str_error		t_str_error;
typedef int32_t					err;
typedef struct s_list			t_list;

enum	e_str_error
{
	E_EXIT_SUCCESS = EXIT_SUCCESS,
	E_ALLOC,
	E_PARAM,
	E_NPOS,
	E_ATOI_FAIL,
};

struct s_str
{
//private:
	uint64_t		_str_identifier;
	void			*_garbage_coll_node;
//public:
	t_str_methods	*m;
	t_str			*(*set)(t_str *, int32_t);
	char			*buff;
	uintptr_t		begin;
	uintptr_t		half;
	uintptr_t		end;
	int32_t			npos;
	int32_t			len;
	int32_t			capacity;
	int32_t			i;
	t_str_error		err;
};

struct s_str_methods
{
	STR_METHODS
};

#endif

typedef struct s_fd	t_fd;
typedef struct s_list	t_list;
typedef struct s_list2	t_list2;
typedef struct s_manage_fds	t_manage_fds;

enum e_charsets
{
	ALPHA,
	UPPER_ALPHA,
	LOWER_ALPHA,
	DIGIT,
	ALPHANUM,
	EXCLUDE,
	INCLUDE,
};

int64_t		ft_atoi(const char *nptr);
void		ft_bzero(void *s, size_t n);
void		*ft_calloc(size_t nmemb, size_t size);
int			ft_isalnum(int c);
int			ft_isalpha(int c);
int			ft_isascii(int c);
int			ft_isdigit(int c);
int			ft_isprint(int c);
char		*ft_itoa(int num);
void		*ft_memchr(const void *s, int c, size_t n);
int			ft_memcmp(const void *s1, const void *s2, size_t n);
void		*ft_memcpy(void *dest, const void *src, size_t n);
void		*ft_memmove(void *dest, const void *src, size_t n);
void		*ft_memset(void *s, int c, size_t n);
char		**ft_split(char const *s, char c);
char		*ft_strchr(const char *s, int c);
char		*ft_strrchr(const char *s, int c);
char		*ft_strdup(const char *str);
size_t		ft_strlcat(char *dst, const char *src, size_t size);
void		ft_striteri(char *s, void (*f)(unsigned int, char*));
size_t		ft_strlcpy(char *dst, const char *src, size_t size);
char		*ft_strjoin(char const *s1, char const *s2);
char		*ft_strjoin_free(char *s1, char *s2, int mode);
size_t		ft_strlen(const char *s);
char		*ft_strmapi(char const *s, char (*f)(unsigned int, char));
int			ft_strncmp(const char *s1, const char *s2, size_t n);
char		*ft_strnstr(const char *big, const char *little, size_t len);
char		*ft_strtrim(char const *s1, char const *set);
char		*ft_substr(char const *s, unsigned int start, size_t len);

int			sub_strlen(const char *s, const char *charset, int mode);
int			sub_strcpy(char *dest, const char *src, const char *charset, int mode);

//		LIST

t_list		*lst_new(void *content);
void		lst_front(t_list **lst, t_list *new);
int			lst_size(t_list *lst);
t_list		*lst_last(t_list *lst);
void		lst_back(t_list **lst, t_list *new);
void		lst_delone(t_list *lst, void (*del)(void *));
void		lst_clear(t_list **lst, void (*del)(void *));
void		lst_iter(t_list *lst, void (*f)(void *));

t_list2		*lst2_new(void *content1, void *content2);
void		lst2_front(t_list2 **lst, t_list2 *new);
int			lst2_size(t_list2 *lst);
t_list2		*lst2_last(t_list2 *lst);
void		lst2_back(t_list2 **lst, t_list2 *new);
void		lst2_delone(t_list2 *lst, void (*del)(void *));
void		lst2_clear(t_list2 **lst, void (*del)(void *));
void		lst2_iter(t_list2 *lst, void (*f)(void *));

//		PRINTF

void	ft_putchar_fd(char c, t_fd fd);
void	ft_putendl_fd(char *s, t_fd fd);
void	ft_putnbr_fd(int n, t_fd fd);
void	ft_putstr_fd(char *s, t_fd fd);
int     tfd_printf(t_fd fd, const char *str, ...);
int     fd_printf(int fd, const char *str, ...);
int     ft_printf(const char *str, ...);
int     err_printf(const char *str, ...);
int     str_printf(t_str *buff, const char *str, ...);

int			cut_string(char *string, size_t start, size_t end);

//	FILE

t_fd			openfd(const char *filename, const char *permissions);
int				ft_open(const char *filename, const char *perm);
t_manage_fds	*fd_database(bool);
int				get_filedata(t_fd *fd, char **filename);
void			del_filedata(void);
int				switch_filedata(t_fd fd);
int				fd_indexation(void);
void			closefd(t_fd fd);
int				read_curr(t_manage_fds *data, int count);
int				readfd(t_fd	fd, char *buff, int count);
char			*get_static_buffer(int fd, bool reset, bool reset_all);
char			*gnl();
int				writefd(t_fd fd, const char *s, size_t len);
int				reset_fd(t_fd fd);

// DAFT

int 	daft_init(const char *path, const char *settings_filename);
void	daft_quit(void);
void	daft_swap(int enum_id);
void	daft_free(void *mem);
void	*daft_get(const char *search);
int		daft_load(void);
void	*daft_edit(const char *search, int size, int mtr_number);
void	*daft_append(const char *key, int size, int mtr_number);

#endif
