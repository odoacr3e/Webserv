/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_hash.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alerusso <alerusso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 23:17:49 by alerusso          #+#    #+#             */
/*   Updated: 2026/02/09 10:47:33 by alerusso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "map_internal.h"

static size_t	_murmurhash2(const void *key, int len);

size_t	_map_default_hash(const char *key)
{
	return (_murmurhash2(key, ft_strlen(key)));
}

//murmurhash2
//https://en.wikipedia.org/wiki/MurmurHash
static size_t	_murmurhash2(const void *key, int len)
{
	uint32_t		m;
	uint32_t		h;
	uint32_t		k;
	unsigned char	*data;

	m = 0x5bd1e995;
	h = DEFAULT_SEED ^ len;
	data = (unsigned char *)key;
	while (len >= 4)
	{
		k = *(uint32_t *)data;
		k *= m;
		k ^= k >> 24;
		k *= m;
		h *= m;
		h ^= k;
		data += 4;
		len -= 4;
	}
	if (len == 3)
		h ^= data[2] << 16;
	if (len >= 2)
		h ^= data[1] << 8;
	if (len >= 1)
	{
		h ^= data[0];
		h *= m;
	}
	h ^= h >> 13;
	h *= m;
	h ^= h >> 15;
	return ((size_t)h);
}
