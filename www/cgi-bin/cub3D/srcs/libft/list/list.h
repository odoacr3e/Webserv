/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alerusso <alessandro.russo.frc@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 19:34:47 by alerusso          #+#    #+#             */
/*   Updated: 2026/02/10 14:15:21 by alerusso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIST_H
# define LIST_H

# include "../libft.h"

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

void	lst_back(t_list **lst, t_list *new);
void	lst2_back(t_list2 **lst, t_list2 *new);
void	lst_front(t_list **lst, t_list *new);
void	lst2_front(t_list2 **lst, t_list2 *new);
void	lst_clear(t_list **lst, void (*del)(void *));
void	lst2_clear(t_list2 **lst, void (*del)(void *));
void	lst_delone(t_list *lst, void (*del)(void *));
void	lst2_delone(t_list2 *lst, void (*del)(void *));
void	*lst_get(t_list *lst, int32_t n);
void	*lst2_get(t_list2 *lst, int32_t n);
void	lst_iter(t_list *lst, void (*f)(void *));
void	lst2_iter(t_list2 *lst, void (*f)(void *));
t_list	*lst_last(t_list *lst);
t_list2	*lst2_last(t_list2 *lst);
t_list	*lst_new(void *content);
t_list2	*lst2_new(void *content1, void *content2);
int	lst_size(t_list *lst);
int	lst2_size(t_list2 *lst);


#endif