/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adoussau <adoussau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/20 22:03:30 by adoussau          #+#    #+#             */
/*   Updated: 2017/12/03 23:10:29 by adoussau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_malloc.h"

t_block		*split_block(t_block *block, size_t size)
{
	size_t	size_new;
	t_block	*new_blocks;

	size_new = block->size - sizeof(t_block) - size;
	block->size = size;
	block->state = 0x01;
	new_blocks = (t_block*)(((void*)(block + 1)) + size);
	new_blocks->next = block->next;
	if (new_blocks->next)
		new_blocks->next->prev = new_blocks;
	block->next = new_blocks;
	new_blocks->size = size_new;
	new_blocks->prev = block;
	new_blocks->state = 0x00;
	return (new_blocks);
}

t_block		*allocate_block(t_block *block, size_t size)
{
	if (block->size > (size + sizeof(t_block) * 2))
		return (split_block(block, size));
	else
		block->state = 0x01;
	return (block);
}

t_block		*get_free_block(t_page *p, size_t s)
{
	t_block *ptr_block;

	ptr_block = p->data;
	while (ptr_block != NULL)
	{
		if (!ptr_block->state && (ptr_block->size > (s + (sizeof(t_block)))))
			return (ptr_block);
		ptr_block = ptr_block->next;
	}
	return (NULL);
}

void		merge_free_block(t_block *b)
{
	t_block *last;

	last = b;
	while (b->prev && !b->prev->state)
		b = b->prev;
	while (last->next && !last->next->state)
		last = last->next;
	if (b == last)
		b->state = 0;
	else
	{
		b->size = ((char*)(last + 1)) + last->size - ((char*)(b + 1));
		b->next = last->next;
		b->state = 0;
		if (last->next)
			last->next->prev = b;
	}
}
