/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   page.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adoussau <adoussau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/20 22:03:30 by adoussau          #+#    #+#             */
/*   Updated: 2017/12/03 23:10:29 by adoussau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_malloc.h"
#include <sys/mman.h>

size_t		get_page_size(size_t s)
{
	if (s <= TINY_MAX_ALLOC)
		return (TINY_SIZE);
	else if (s <= SMALL_MAX_ALLOC)
		return (SMALL_SIZE);
	else
		return (s + sizeof(t_page) + sizeof(t_block) * 1);
}

size_t		get_page_type(size_t s)
{
	if (s <= TINY_MAX_ALLOC)
		return (TINY_TYPE);
	else if (s <= SMALL_MAX_ALLOC)
		return (SMALL_TYPE);
	else
		return (LARGE_TYPE);
}

t_page		*allocate_new_page(size_t s)
{
	t_page	*new_page;
	t_block *block;
	size_t	page_size;

	page_size = get_page_size(s);
	new_page = (t_page*)mmap(
		NULL,
		page_size,
		PROT_READ | PROT_WRITE,
		MAP_PRIVATE | MAP_ANON,
		-1,
		0);
	if (new_page)
	{
		new_page->type = get_page_type(s);
		new_page->size = page_size - sizeof(t_page);
		block = (t_block *)(new_page + 1);
		block->size = page_size - sizeof(t_block) - sizeof(t_page);
		new_page->data = block;
		return (new_page);
	}
	return (NULL);
}

void		free_page(t_page *page)
{
	if (page == g_pages)
	{
		g_pages = page->next;
		if (page->next)
			page->next->prev = NULL;
	}
	else if (page->prev)
	{
		page->prev->next = page->next;
		if (page->next)
			page->next->prev = page->prev;
	}
	munmap(page, page->size + sizeof(t_page));
}
