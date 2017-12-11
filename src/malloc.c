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

#include <stdio.h>
#include <math.h>
#include "libft_malloc.h"
#include <sys/mman.h>
#include <string.h>

t_page		*g_pages = NULL;

t_block		*split_block(t_block *block, size_t size)
{
	size_t size_new;
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

void	*malloc(size_t s)
{
	t_page *ptr_page;
	t_page *new_page;
	t_block *block;

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

void	print_block(t_block *b)
{
	int size;
	int i;

	malloc_putstr("\t\t----------Block-----------\n\t\tSize:  ");
	malloc_putnbr(b->size);
	malloc_putstr("  (real: ");
	malloc_putnbr(b->size + sizeof(t_block));
	malloc_putstr(")\n\t\tState: ");
	malloc_putnbr(b->state);
	malloc_putstr("\n\t\tData:  ");
	size = b->size < 25 ? b->size : 25;
	i = 0;
	while (i < size)
	{
		malloc_printhex(*(((unsigned char *)(b + 1)) + i));
		write(1, " ", 1);
	}
	write(1, "\n", 1);
}

void	print_page(t_page *p)
{
	t_block *b;

	malloc_putstr("\t------------Page------------\n\tSize: ");
	malloc_putnbr(p->size);
	malloc_putstr("  (real: ");
	malloc_putnbr(p->size + sizeof(t_page));
	malloc_putstr(")\n\ttype: ");
	if (p->type == 1)
		malloc_putstr("TINY\n");
	if (p->type == 2)
		malloc_putstr("SMALL\n");
	if (p->type == 3)
		malloc_putstr("LARGE\n");
	b = p->data;
	while (b)
	{
		print_block(b);
		b = b->next;
	}
}

void	print_pages(t_page *p)
{
	malloc_putstr("\n---- PRINT PAGE ----\n");
	while (p)
	{
		print_page(p);
		p = p->next;
	}
}

t_block	*search_block(void *ptr, t_page **page)
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

void	merge_free_block(t_block *b)
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

void	free_page(t_page *page)
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

void	free(void *ptr)
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

void	*realloc(void *ptr, size_t size)
{
	t_page	*ptr_page;
	t_block	*ptr_block;
	char	*new_ptr;
	t_block	*new_block;

	ptr_block = search_block(ptr, &ptr_page);
	if (ptr_block)
	{
		if (ptr_block->size >= size)
		{
			return (ptr);
		}
		else
		{
			if (ptr_block->next && ptr_block->next->state == 0
				&& (size <= (ptr_block->size + ptr_block->next->size)))
			{
				new_block = (t_block *)(((char *)(ptr_block + 1)) +
				size);
				new_block->size = ptr_block->next->size -
				(size - ptr_block->size);
				new_block->next = ptr_block->next->next;
				new_block->prev = ptr_block;
				ptr_block->next = new_block;
				ptr_block->size = size;
				if (new_block->next)
					new_block->next->prev = new_block;
			}
			else
			{
				new_ptr = malloc(size);
				if (new_ptr)
				{
					memcpy(new_ptr, ptr_block + 1, ptr_block->size);
					free(ptr);
					return (new_ptr);
				}
			}
		}
	}
	return (NULL);
}

void	show_alloc_mem(void)
{
	print_pages(g_pages);
}
