/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adoussau <adoussau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/20 22:03:30 by adoussau          #+#    #+#             */
/*   Updated: 2017/12/03 22:53:21 by adoussau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "libft.h"
#include <stdio.h>
#include <math.h>
#include "libft_malloc.h"
#include <sys/mman.h>
#include <string.h>
#include "print.h"

t_page *g_pages = NULL;

t_block *split_block(t_block *block, size_t size) {
	size_t size_new;
	t_block	*new_blocks;

	size_new = block->size - sizeof(t_block) - size;
	block->size = size;
	block->state = 0x01;

	new_blocks = (t_block*)(((void*)(block + 1)) + size);

	new_blocks->next = block->next;
	block->next = new_blocks;

	new_blocks->size = size_new;
	new_blocks->prev = block;
	new_blocks->state = 0x00;
	return (new_blocks);
}

t_block *allocate_block(t_block *block, size_t size) {
	if (block->size > (size + sizeof(t_block) * 2))
		return (split_block(block, size));
	else
		block->state = 0x01;
	return (block);
}

t_block *getFreeBlock(t_page *p, size_t s) {
	t_block *ptr_block;

	ptr_block = p->data;
	while (ptr_block != NULL) {
		if (!ptr_block->state && (ptr_block->size > (s + (sizeof(t_block)))))
			return (ptr_block);
		ptr_block = ptr_block->next;
	}
	return (NULL);
}

size_t getPageSize(size_t s) {
	if (s <= TINY_MAX_ALLOC)
		return (TINY_SIZE);
	else if (s <= SMALL_MAX_ALLOC)
		return (SMALL_SIZE);
	else
		return (s + sizeof(t_page) + sizeof(t_block) * 1);
}

size_t getPageType(size_t s) {
	if (s <= TINY_MAX_ALLOC)
		return (TINY_TYPE);
	else if (s <= SMALL_MAX_ALLOC)
		return (SMALL_TYPE);
	else
		return (LARGE_TYPE);
}


t_page	*allocate_new_page(size_t s) {
	t_page *new_page;
	size_t page_size = getPageSize(s);

	new_page = (t_page*)mmap(
		NULL,
		page_size,
		PROT_READ | PROT_WRITE,
		MAP_PRIVATE | MAP_ANON,
		-1,
		0
	);
	if (new_page) {
		new_page->type = getPageType(s);
		new_page->size = page_size - sizeof(t_page);
		// new_page->prev = NULL;
		// new_page->next = NULL;
		// new_page->data = NULL;

		t_block *block = (t_block *)(new_page + 1);
		block->size = page_size - sizeof(t_block) - sizeof(t_page);
		// block->prev = NULL;
		// block->next = NULL;
		// block->state = 0;

		new_page->data = block;

		return (new_page);
	}
	return (NULL);
}

void *malloc(size_t s) {
	t_page *ptr_page = NULL;
	t_block *block;

	ptr_page = g_pages;
	char page_type = getPageType(s);

	if (page_type != LARGE_TYPE) {
		while (ptr_page) {
			if (ptr_page->type == page_type)
				if ((block = getFreeBlock(ptr_page, s)))
					return (allocate_block(block, s) + 1);
			ptr_page = ptr_page->next;
		}
	}

	t_page *new_page = allocate_new_page(s);
	if (new_page) {
		allocate_block(new_page->data, s);
		if (g_pages == NULL)
			g_pages = new_page;
		else {
			new_page->next = g_pages;
			g_pages->prev = new_page;
			g_pages = new_page;
		}
		return (new_page->data + 1);
	}
	return (NULL);
}


void print_block(t_block *b) {
	putstr("\t\t----------Block-----------\n\t\tSize:  ");
	putnbr(b->size);
	putstr("  (real: ");
	putnbr(b->size + sizeof(t_block));
	putstr(")\n\t\tState: ");
	putnbr(b->state);
	putstr("\n\t\tData:  ");

	int size = b->size < 25 ? b->size : 25;
	for (int i=0; i<size; i++) {
		printhex(*(((unsigned char *)(b+1)) + i));
		write(1," ",1);
	}
	write(1,"\n",1);
}

void print_page(t_page *p) {
	putstr("\t------------Page------------\n\tSize: ");
	putnbr(p->size);
	putstr("  (real: ");
	putnbr(p->size + sizeof(t_page));
	putstr(")\n\ttype: ");
	switch (p->type) {
		case 1:
			putstr("TINY\n");
			break;
		case 2:
			putstr("SMALL\n");
			break;
		case 3:
			putstr("LARGE\n");
			break;
	}
	t_block *b = p->data;
	while (b) {
		print_block(b);
		b = b->next;
	}
}

void print_pages(t_page *p) {
	putstr("\n---- PRINT PAGE ----\n");
	while (p) {
		print_page(p);
		p = p->next;
	}
}


t_block *search_block(void* ptr, t_page **page) {
	t_page *ptr_page = g_pages;
	t_block *ptr_block;
	while (ptr_page) {
		ptr_block = ptr_page->data;
		while (ptr_block) {
			if (ptr_block + 1 == ptr) {
				*page = ptr_page;
				return (ptr_block);
			}
			ptr_block = ptr_block->next;
		}
		ptr_page = ptr_page->next;
	}
	return (NULL);
}

void merge_free_block(t_block *b) {
	t_block *last;
	last = b;

	while (b->prev && !b->prev->state)
		b = b->prev;
	while (last->next && !last->next->state)
		last = last->next;
	if (b == last)
		b->state = 0;
	else {
		b->size = ((char*)(last+1)) + last->size - ((char*)(b+1));
		b->next = last->next;
		b->state = 0;
		if (last->next)
			last->next->prev = b;
	}
}

void free_page(t_page *page) {
	if (page == g_pages) {
		g_pages = page->next;
		if (page->next)
			page->next->prev = NULL;
	}
	else if (page->prev) {
		page->prev->next = page->next;
		if (page->next)
			page->next->prev = page->prev;
	}
	munmap(page, page->size + sizeof(t_page));
}


void free(void *ptr) {
	t_page *ptr_page;
	t_block *ptr_block;

	if (ptr == NULL)
		return;

	ptr_block = search_block(ptr, &ptr_page);
	if (ptr_block) {
		merge_free_block(ptr_block);
		t_block *b = ptr_page->data;
		if (b && !b->state && !b->next)
		free_page(ptr_page);
	}
}

void *realloc(void *ptr, size_t size) {
	t_page *ptr_page;
	t_block *ptr_block;
	ptr_block = search_block(ptr, &ptr_page);
	if (ptr_block) {
		if (ptr_block->size >= size) {
			return (ptr);
		} else {
			if (ptr_block->next && ptr_block->next->state == 0 && (size <= ( ptr_block->size + ptr_block->next->size))) {
				t_block *new_block = (t_block *)(((char *)(ptr_block + 1)) + size);
				new_block->size = ptr_block->next->size - (size  - ptr_block->size);
				new_block->next = ptr_block->next->next;
				new_block->prev = ptr_block;

				ptr_block->next = new_block;
				ptr_block->size = size;
				if (new_block->next)
					new_block->next->prev = new_block;
			} else {
				char *new_ptr = malloc(size);
				if (new_ptr) {
					memcpy(new_ptr, ptr_block + 1, ptr_block->size);
					free(ptr);
					return (new_ptr);
				}
			}
		}
	}
	return (NULL);
}

void show_alloc_mem() {
	print_pages(g_pages);
}
