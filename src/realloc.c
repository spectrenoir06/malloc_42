/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adoussau <adoussau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/20 22:03:30 by adoussau          #+#    #+#             */
/*   Updated: 2018/01/05 18:25:56 by adoussau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_malloc.h"

void		malloc_memcpy(char *ptr1, char *ptr2, size_t s)
{
	while (s--)
		*ptr1++ = *ptr2++;
}

t_block		*split_block_rea(t_block *ptr_block, size_t s)
{
	t_block	*new_block;

	new_block = (t_block *)(((char *)(ptr_block + 1)) + s);
	new_block->size = ptr_block->next->size - (s - ptr_block->size);
	new_block->next = ptr_block->next->next;
	new_block->prev = ptr_block;
	ptr_block->next = new_block;
	ptr_block->size = s;
	if (new_block->next)
		new_block->next->prev = new_block;
	return (ptr_block);
}

void		*rea_cpy(t_block *block, size_t s)
{
	char	*rea;

	if ((rea = malloc(s)))
	{
		malloc_memcpy(rea, (char *)(block + 1), block->size);
		free(block + 1);
		return (rea);
	}
	return (NULL);
}

void		*realloc(void *ptr, size_t s)
{
	t_page	*ptr_page;
	t_block	*ptr_block;

	if (ptr == NULL)
		return (malloc(s));
	ptr_block = search_block(ptr, &ptr_page);
	if (ptr_block)
	{
		if (ptr_block->size >= s)
			return (ptr);
		else
		{
			if (ptr_block->next && ptr_block->next->state == 0
			&& (s <= (ptr_block->size + ptr_block->next->size)))
				return (split_block_rea(ptr_block, s) + 1);
			else
				return (rea_cpy(ptr_block, s));
		}
	}
	return (NULL);
}
