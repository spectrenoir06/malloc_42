/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adoussau <adoussau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/20 22:03:30 by adoussau          #+#    #+#             */
/*   Updated: 2017/11/29 22:49:48 by adoussau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>
#include <math.h>
#include "malloc.h"
#include <sys/mman.h>

t_page *pages = NULL;

void split_block(t_page *page, t_block *block, size_t size) {
	// printf("Split bloc	k\n");
	size_t size_new = block->size - sizeof(t_block) - size;

	// printf("splitblock: block->size: %ld size: %ld size_new: %ld\n",block->size, size,size_new);

	t_block	*new_blocks = NULL;

	block->size = size;
	block->state = 0x01;

	new_blocks = (t_block*)(((void*)(block + 1)) + size);

	new_blocks->next = block->next;
	block->next = new_blocks;

	new_blocks->size = size_new;
	new_blocks->prev = block;
	new_blocks->state = 0x00;

	page->blocks_nb++;
}

void allocate_block(t_page *page, t_block *block, size_t size) {
	if (block->size > (size + sizeof(t_block) * 2))
		split_block(page, block, size);
	else {
		block->state = 0x01;
	}

}

t_block *getFreeBlock(t_page *p, size_t s) {
	t_block *ptr_block = NULL;

	ptr_block = p->data;
	while (ptr_block != NULL) {
		// printf("Block Found: data=%d, block_size=%d a:%d\n", ptr_block->size, s, s + (sizeof(t_block) * 2));
		if (!ptr_block->state && (ptr_block->size > (s + (sizeof(t_block))))) {
			return ptr_block;
		}
		ptr_block = ptr_block->next;
	}
	return NULL;
}

size_t getPageSize(size_t s) {
	if (s <= TINY_MAX_ALLOC)
		return (TINY_SIZE);
	else if (s <= SMALL_MAX_ALLOC)
		return (SMALL_SIZE);
	else
		return (s + sizeof(t_page) + sizeof(t_block)*1);
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
	// printf("New Page\n");
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
		//bzero(new_page, s + sizeof(t_page)+ sizeof(t_block));
		new_page->type = getPageType(s);
		new_page->size = page_size - sizeof(t_page);
		// new_page->prev = NULL;
		new_page->next = NULL;
		new_page->data = NULL;

		// printf("New block\n");
		t_block *block = (t_block *)(new_page + 1);
		// block->magic =  0x42;
		// block->magic2 = 0x43;
		block->size = page_size - sizeof(t_block) - sizeof(t_page);
		block->prev = NULL;
		block->next = NULL;
		block->state = 0;
		new_page->blocks_nb = 1;

		new_page->data = block;

		return (new_page);
	}
	return NULL;
}

void *ft_malloc(size_t s) {
	t_page *ptr_page = NULL;
	// t_page *last_page = NULL;

	ptr_page = pages;
	char page_type = getPageType(s);

	if (page_type != LARGE_TYPE) {
		while (ptr_page) {
			//printf("Pages Found\n");
			if (ptr_page->type == page_type) {
				t_block *block = getFreeBlock(ptr_page, s);
				if (block != NULL) {
					allocate_block(ptr_page, block, s);
					return (block+1);
				}
			}
			// last_page = ptr_page;
			ptr_page = ptr_page->next;
		}
	}

	t_page *new_page = allocate_new_page(s);
	if (new_page) {
		allocate_block(new_page, new_page->data, s);
		if (pages == NULL)
			pages = new_page;
		else {
			new_page->next = pages;
			//pages->prev = new_page;
			pages = new_page;
		}
		return (new_page->data + 1);
	}
	return (NULL);
}


void print_block(t_block *b) {
	printf("\t\t----------Block-----------\n");
	printf("\t\tSize:  %zu  (real: %zu)\n", b->size, b->size + sizeof(t_block));
	printf("\t\tState: %d\n", b->state);
	printf("\t\tData:  %p\n", (b + 1));
	printf("\t\tData:  \"%s\"\n", (char *)(b + 1));
	printf("\t\tData:  ");

	int size = b->size < 25 ? b->size : 25;
	for (int i=0; i<size; i++) {
		printf("%02x ",*(((unsigned char *)(b+1)) + i));
	}
	printf("\n");
}

void print_page(t_page *p) {
	printf("\t------------Page------------\n");
	printf("\tSize:   %ld  (real: %ld)\n", p->size, p->size + sizeof(t_page));
	printf("\tblocks: %d\n", p->blocks_nb);
	printf("\ttype:   %d (",p->type);
	switch (p->type) {
		case 1:
			printf("TINY)\n");
			break;
		case 2:
			printf("SMALL)\n");
			break;
		case 3:
			printf("LARGE)\n");
			break;
	}
	t_block *b = p->data;
	while (b) {
		print_block(b);
		b = b->next;
	}
}

void print_pages(t_page *p) {
	printf("\n---- PRINT PAGE ----\n");
	while (p) {
		print_page(p);
		p = p->next;
	}
}


t_block *search_block(void* ptr, t_page **page) {
	t_page *ptr_page = pages;
	t_block *ptr_block;
	while (ptr_page) {
		ptr_block = ptr_page->data;
		while (ptr_block) {
			if (ptr_block + 1 == ptr) {
				// printf("block found for %p\n",ptr);
				// print_block(ptr_block);
				*page = ptr_page;
				return (ptr_block);
			}
			ptr_block = ptr_block->next;
		}
		ptr_page = ptr_page->next;
	}
	return (NULL);
}



void ft_free(void *ptr) {
	t_page *ptr_page;
	t_block *ptr_block;

	if (ptr == NULL)
		return;

	ptr_block = search_block(ptr, &ptr_page);
	if (ptr_block) {
		ptr_block->state = 0;
	}
}

void *ft_realloc(void *ptr, size_t size) {
	t_page *ptr_page;
	t_block *ptr_block;
	ptr_block = search_block(ptr, &ptr_page);
	if (ptr_block) {
		if (ptr_block->size >= size) {
			return (ptr);
		} else {
			if (ptr_block->next && ptr_block->next->state == 0 && (size <= ( ptr_block->size + ptr_block->next->size))) {
				// printf("merge\n");
				t_block *new_block = (t_block *)(((char *)(ptr_block + 1)) + size);
				new_block->size = ptr_block->next->size - (size  - ptr_block->size);
				new_block->next = ptr_block->next->next;
				new_block->prev = ptr_block;

				ptr_block->next = new_block;
				ptr_block->size = size;
				if (new_block->next)
					new_block->next->prev = new_block;
			}
			else {
				char *ptr = ft_malloc(size);
				if (ptr) {
					memcpy(ptr, ptr_block + 1, ptr_block->size);
					ptr_block->state = 0;
					return (ptr);
				}
			}
		}
	}
	return (NULL);
}

int		main(int argc, char **argv)
{
	(void)argc;
	(void)argv;
	printf("getpagesize() = %d\n", getpagesize());

	printf("sizeof(t_page)=%ld\n" , sizeof(t_page));
	printf("sizeof(t_block)=%ld\n", sizeof(t_block));

	// char* tab[100];
	//
	// for (int i=0;i<10;i++) {
	// 	tab[i] = (char*)ft_malloc(60);
	// }
	// for (int i=0; i<10; i++) {
	// 	for (int j=0; j<50; j++)
	// 		tab[i][j] = ('a'+ i);
	// 	tab[i][50] = 0;
	//
	// }
	// ft_malloc(500);

	char * str = ft_malloc(10);

	memcpy(str,"012345678\0",10);

	// ft_malloc(10);
	// ft_malloc(10);
	// ft_malloc(10);
	// ft_malloc(10);
	// ft_malloc(10);

	// ft_malloc(1025);

	print_pages(pages);

	ft_realloc(str, 20);
	//
	// for (int i=0; i < 10; i++) {
	// 	ft_realloc(str, i*10);
	// 	print_page(pages);
	// }

	// memcpy(str,"0123456789012345678\0", 20);

	// ft_free(tab[5]);

	// ft_malloc(10);
	// ft_malloc(10);
	// ft_malloc(10);
	//
	print_pages(pages);


	// memcpy(str2, "1111111111111111", 16);
	// memcpy(str3, "0123456789012345", 16);
	// memcpy(str4, "0123456789012345", 16);

	// memcpy(str, "0123456789", 10);

	return (0);
}
