/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adoussau <adoussau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/20 22:03:30 by adoussau          #+#    #+#             */
/*   Updated: 2017/11/21 17:25:03 by adoussau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>
#include <math.h>
#include "malloc.h"
#include <sys/mman.h>

t_page *pages = NULL;

#define page_size 4096*2


void split_block(t_block *block, size_t size) {
	size_t size_new = block->size - sizeof(t_block) - size;
	t_block	*new_blocks = NULL;

	block->size = size;
	block->state = 0x01;

	new_blocks = (t_block*)(((void *)(block + 1)) + size);

	block->next = new_blocks;

	new_blocks->size = size_new;
	new_blocks->next = NULL;
	new_blocks->prev = block;
	new_blocks->state = 0;
}

t_block *getFreeBlock(t_page *p, size_t s) {
	t_block *ptr_block = NULL;

	ptr_block = p->data;
	while (ptr_block != NULL) {
		if (!ptr_block->state && (ptr_block->size > (s + sizeof(t_block)))) {
			return ptr_block;
		}
		printf("Block used\n");
		ptr_block = ptr_block->next;
	}
	return NULL;
}

t_block *malloc_test2(size_t s) {

	t_page *ptr_page = NULL;

	if (pages != NULL) {
		ptr_page = pages;
		while (ptr_page) {
			printf("Pages Found\n");
			t_block *block = getFreeBlock(ptr_page, s);
				if (block != NULL) {
					printf("Block found\n");
					split_block(block, s);
					ptr_page->blocks_nb++;
					return (block+1);
				}
			ptr_page = ptr_page->next;
		}
	}

}


void	*malloc_test(size_t s) {
	if (pages == NULL) {
		t_page *ptr;
		printf("New Page\n");

		ptr = (t_page*)mmap(
								NULL,
								page_size + sizeof(t_page),
								PROT_READ | PROT_WRITE,
								MAP_PRIVATE | MAP_ANON,
								-1,
								0
							);
		if (ptr) {
			bzero(ptr,page_size+sizeof(t_page));
			ptr->type = 42;
			ptr->size = page_size;
			ptr->prev = NULL;
			ptr->next = NULL;
			ptr->data = NULL;
			ptr->blocks_nb = 0xffffffff;

			t_block *block = ptr + 1;
			block->size = page_size - sizeof(t_block);
			block->prev = NULL;
			block->next = NULL;
			block->state = 0;

			ptr->data = block;
			ptr->blocks_nb++;
			printf("New block\n");
			pages = ptr;
			return (ptr->data + 1);
		}
	}
}
int		main(int argc, char **argv)
{
	printf("%d\n", getpagesize());
	malloc_test(10);
	char *str1 = malloc_test2(10);
	char *str2 = malloc_test2(10);
	char *str3 = malloc_test2(10);
	char *str4 = malloc_test2(10);

	memcpy(str1, "0000000000", 10);
	memcpy(str2, "1111111111", 10);
	memcpy(str3, "012345678\0", 10);
	memcpy(str4, "012345678\0", 10);

	// memcpy(str, "0123456789", 10);

	return (0);
}
