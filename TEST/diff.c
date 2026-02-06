#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <fcntl.h>
#include <stdbool.h>

#define FILE_1 "../PORNO_EMMA_WATSON.ico"
#define FILE_2 "../www/var/favicon.ico"
#define SIZE 233816

typedef struct s_diff
{
	int		fd;
	char	bf[SIZE + 3];
	char	*tmp;
	int		tmplen;
	int		left;
}		t_diff;

static char	*itoa(int n, int *len);
static bool	ft_getline(t_diff *f);
static int	bin_strcmp(char *tmp1, char *tmp2, int len1, int len2);
static void	print_diff(t_diff *f1, t_diff *f2, int newline_counter);
static int	end(t_diff *f1, t_diff *f2, int ecode);

int	main()
{
	t_diff	f1;
	t_diff	f2;
	int		newline_counter;

	f1 = (t_diff){0};
	f2 = (t_diff){0};
	f1.fd = open(FILE_1, O_RDONLY);//"../PORNO_EMMA_WATSON.ico"
	f2.fd = open(FILE_2, O_RDONLY);//"../www/var/favicon.ico"
	newline_counter = 0;
	if (!f1.fd || !f2.fd)
		return (end(&f1, &f2, 1));
	if (read(f1.fd, f1.bf, SIZE) != SIZE)
		//{;}
		return (end(&f1, &f2, 2));
	if (read(f2.fd, f2.bf, SIZE) != SIZE)
		return (end(&f1, &f2, 3));
	f1.left = SIZE + 3, f2.left = SIZE + 3; 
	f1.bf[SIZE] = 'E', f2.bf[SIZE] = 'E';
	f1.bf[SIZE + 1] = 'O', f2.bf[SIZE + 1] = 'O';
	f1.bf[SIZE + 2] = 'F', f2.bf[SIZE + 2] = 'F';
	for (;f1.left > 0;)
	{
		if (ft_getline(&f1) != 0)
			return (end(&f1, &f2, 4));
		if (ft_getline(&f2) != 0)
			return (end(&f1, &f2, 5));
		if (bin_strcmp(f1.tmp, f2.tmp, f1.tmplen, f2.tmplen))
			print_diff(&f1, &f2, newline_counter);
		// else
		// 	write(1, "DIFF_OK!\n", 9);
		free(f1.tmp);
		free(f2.tmp);
		newline_counter++;
	}
	end(&f1, &f2, 0);
}

/*
			h_len = header.tellg();
			bytes -= (int)h_len; //161 -> left
			//request.setBodyLen(request.getBodyLen() - )
			for (int i = 0; i != bytes; i++)
				buf[i] = buf[i + h_len];
*/
static bool	ft_getline(t_diff *f)
{
	int	endline;//h_len

	endline = 0;
	while (f->bf[endline] != '\n' && \
	f->bf[endline] != 'E' && f->bf[endline] != 'O' && f->bf[endline] != 'F')
	{
		++endline;
	}
	f->tmplen = endline;
	++endline;
	f->left -= endline;//trim
	f->tmp = malloc(endline);
	if (!f->tmp)
		return (1);
	for (int i = 0; i != endline; i++)
		f->tmp[i] = f->bf[i];
	for (int i = 0; i != f->left; i++)
		f->bf[i] = f->bf[i + endline];
	return (0);
}

static int	bin_strcmp(char *tmp1, char *tmp2, int len1, int len2)
{
	int	i;

	for (i = 0; i != len1 && i != len2; i++)
	{
		if (tmp1[i] != tmp2[i])
			return (tmp1[i] - tmp2[i]);
	}
	return (tmp1[i] - tmp2[i]);
}

static void	print_diff(t_diff *f1, t_diff *f2, int newline_counter)
{
	char	*temp;
	int		len;

	temp = itoa(newline_counter, &len);
	write(1, "diff in line: ", 14);
	write(1, temp, len);
	write(1, "\n", 1);
	free(temp);
	temp = calloc(100, sizeof(char));
	write(1, "Our: |", 6);
	write(1, f1->tmp, f1->tmplen);
	write(1, "|\nOri: |", 8);
	write(1, f2->tmp, f2->tmplen);
	write(1, "|\nhexa dump:\n", 13);
	for (int i = 0; i != f1->tmplen; i++)
	{
		sprintf(temp, "%d", f1->tmp[i]);
		write(1, temp, strlen(temp));
		write(1, " ", 1);
	}
	write(1, "\n---\n", 5);
	for (int i = 0; i != f2->tmplen; i++)
	{
		sprintf(temp, "%d", f2->tmp[i]);
		write(1, temp, strlen(temp));
		write(1, " ", 1);
	}
	write(1, "|\n---------------\n", 18);
	free(temp);
}

static int	end(t_diff *f1, t_diff *f2, int ecode)
{
	close(f1->fd), close(f2->fd);
	switch (ecode)
	{
		case 1:
			printf("cannot open file\n");
			break ;
		case 2:
			printf("cannot read file 1 to the end\n");
			break ;
		case 3:
			printf("cannot read file 2 to the end\n");
			break ;
	}
	return (ecode);
}

static void	reverse(char *str, int len)
{
	int i = 0;
	int j = len - 1;
	char tmp;

	while (i < j)
	{
		tmp = str[i];
		str[i] = str[j];
		str[j] = tmp;
		i++;
		j--;
	}
}

static char	*itoa(int n, int *len)
{
	char	*str;
	int		i = 0;
	int		sign;
	long	nb;

	nb = n;
	sign = 0;
	if (nb < 0)
	{
		sign = 1;
		nb = -nb;
	}
	str = malloc(12);
	if (!str)
		return NULL;
	while (nb > 0)
	{
		str[i++] = (nb % 10) + '0';
		nb /= 10;
	}
	if (i == 0)
		str[i++] = '0';
	if (sign)
		str[i++] = '-';
	str[i] = '\0';
	reverse(str, i);
	*len = i;
	return str;
}