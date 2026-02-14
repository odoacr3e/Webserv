/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edraccan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 18:03:15 by edraccan          #+#    #+#             */
/*   Updated: 2025/04/01 18:01:58 by edraccan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// void	*ft_calloc(size_t nmemb, size_t size)
// {
// 	void	*ptr;

void	*ft_calloc(size_t nmemb, size_t size)
{
	size_t	total_size;
	void	*ptr;

	if (nmemb == 0 || size == 0)
	{
		ptr = malloc(1);
		if (ptr)
			*(unsigned char *)ptr = 0;
		return (ptr);
	}
	total_size = nmemb * size;
	if (total_size / nmemb != size)
		return (NULL);
	ptr = malloc(total_size);
	if (!ptr)
		return (NULL);
	ft_bzero(ptr, total_size);
	return (ptr);
}

/* 
int main()
{
	char *p = ft_calloc(10, sizeof(char));
	int i = 0;
	while (i < 10)
	{
		if (p[i] == 0)
			printf("0");
		i++;
	}
	printf("\n");
} */
