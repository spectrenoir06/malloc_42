/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adoussau <adoussau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/20 22:03:30 by adoussau          #+#    #+#             */
/*   Updated: 2017/11/27 02:57:04 by adoussau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>
#include <math.h>
#include "malloc.h"
#include <sys/mman.h>

t_page *pages = NULL;

#define page_size 4096*2
#define DATA(b,type)	((void*)((char*)b + sizeof(type) + 1))


void split_block(t_page *page, t_block *block, size_t size) {
	printf("Split block\n");
	size_t size_new = block->size - sizeof(t_block) - size;

	printf("splitblock: block->size: %d size: %d size_new: %d\n",block->size, size,size_new);

	t_block	*new_blocks = NULL;

	block->size = size;
	block->state = 0x01;

	new_blocks = (t_block*)(((void*)(block + 1)) + size);

	block->next = new_blocks;

	new_blocks->size = size_new;
	new_blocks->next = NULL;
	new_blocks->prev = block;
	new_blocks->state = 0;

	page->blocks_nb++;
}

t_block *getFreeBlock(t_page *p, size_t s) {
	t_block *ptr_block = NULL;

	ptr_block = p->data;
	while (ptr_block != NULL) {
		if (!ptr_block->state && (ptr_block->size > (s + (sizeof(t_block) * 2)))) {
			printf("Block Found: data=%d, block_size=%d a:%d\n", ptr_block->size, s, s + (sizeof(t_block) * 2));
			return ptr_block;
		}
		ptr_block = ptr_block->next;
	}
	return NULL;
}


t_page	*allocate_new_page(size_t s) {
	t_page *new_page;
	printf("New Page\n");

	new_page = (t_page*)mmap(
		NULL,
		s + sizeof(t_page) + sizeof(t_block),
		PROT_READ | PROT_WRITE,
		MAP_PRIVATE | MAP_ANON,
		-1,
		0
	);
	if (new_page) {
		bzero(new_page, s + sizeof(t_page)+ sizeof(t_block));
		new_page->type = 42;
		new_page->size = s;
		new_page->prev = NULL;
		new_page->next = NULL;
		new_page->data = NULL;

		printf("New block\n");
		t_block *block = (t_block *)(new_page + 1);
		int i = sizeof(t_block);
		// block->magic =  0x42;
		// block->magic2 = 0x43;
		block->size = s - i;
		block->prev = NULL;
		block->next = NULL;
		block->state = 0;
		new_page->blocks_nb = 1;

		new_page->data = block;

		return (new_page);
	}
	return NULL;
}

t_block *malloc_test2(size_t s) {
	t_page *ptr_page = NULL;
	t_page *last_page = NULL;

	ptr_page = pages;
	while (ptr_page) {
		printf("Pages Found\n");
		t_block *block = getFreeBlock(ptr_page, s);
		if (block != NULL) {
			split_block(ptr_page, block, s);
			return (block+1);
		}
		last_page = ptr_page;
		ptr_page = ptr_page->next;
	}

	t_page *new_page = allocate_new_page(200);
	if (new_page) {
		split_block(new_page, new_page->data, s);
		if (pages == NULL)
			pages = new_page;
		else {
			last_page->next = new_page;
			new_page->prev = last_page;
		}
		return (new_page->data + 1);
	}
	return (NULL);
}

int		main(int argc, char **argv)
{
	printf("%d\n", getpagesize());

	printf("sizeof(t_page)=%d\n" , sizeof(t_page));
	printf("sizeof(t_block)=%d\n", sizeof(t_block));

	for (int i=0;i<10;i++) {
		char *str1 = malloc_test2(10);
		memcpy(str1, "abcdefghij", 10);
	}

	//allocate_new_page(500);
	// char *str3 = malloc_test2(10);
	// char *str4 = malloc_test2(10);
	// 	 *str4 = malloc_test2(10);
	// 		 *str4 = malloc_test2(10);
	// 			 *str4 = malloc_test2(10);
	// 				 *str4 = malloc_test2(10);

	// char *str5 = malloc_test2(10);
	//
	// char *str6 = malloc_test2(10);

	//memcpy(str1, "0000000000000000", 16);

	printf("sizeof(int)=%d\n",sizeof(int));
	// memcpy(str2, "1111111111111111", 16);
	// memcpy(str3, "0123456789012345", 16);
	// memcpy(str4, "0123456789012345", 16);

	// memcpy(str, "0123456789", 10);

	return (0);
}
