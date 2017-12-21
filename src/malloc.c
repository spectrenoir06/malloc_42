/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adoussau <adoussau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/20 22:03:30 by adoussau          #+#    #+#             */
/*   Updated: 2017/12/21 16:58:08 by adoussau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <math.h>
#include "libft_malloc.h"
#include <string.h>

t_page		*g_pages = NULL;

void		*has_free_block(size_t s)
{
	t_page	*ptr_page;
	t_block	*block;

	ptr_page = g_pages;
	if (get_page_type(s))
	{
		while (ptr_page)
		{
			if (ptr_page->type == (char)get_page_type(s))
				if ((block = get_free_block(ptr_page, s)))
					return (allocate_block(block, s) + 1);
			ptr_page = ptr_page->next;
		}
	}
	return (NULL);
}

void		*malloc(size_t s)
{
	t_block	*block;
	t_page	*new_page;

	if ((block = has_free_block(s)))
		return (block);
	new_page = allocate_new_page(s);
	if (new_page)
	{
		allocate_block(new_page->data, s);
		if (g_pages == NULL)
			g_pages = new_page;
		else
		{
			new_page->next = g_pages;
			g_pages->prev = new_page;
			g_pages = new_page;
		}
		return (new_page->data + 1);
	}
	return (NULL);
}

t_block		*search_block(void *ptr, t_page **page)
{
	t_page	*ptr_page;
	t_block	*ptr_block;

	ptr_page = g_pages;
	while (ptr_page)
	{
		ptr_block = ptr_page->data;
		while (ptr_block)
		{
			if (ptr_block + 1 == ptr)
			{
				*page = ptr_page;
				return (ptr_block);
			}
			ptr_block = ptr_block->next;
		}
		ptr_page = ptr_page->next;
	}
	return (NULL);
}

void		free(void *ptr)
{
	t_block	*b;
	t_page	*ptr_page;
	t_block	*ptr_block;

	if (ptr == NULL)
		return ;
	ptr_block = search_block(ptr, &ptr_page);
	if (ptr_block)
	{
		merge_free_block(ptr_block);
		b = ptr_page->data;
		if (b && !b->state && !b->next)
			free_page(ptr_page);
	}
}
