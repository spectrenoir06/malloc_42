/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adoussau <adoussau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/20 22:03:30 by adoussau          #+#    #+#             */
/*   Updated: 2017/11/21 04:58:40 by adoussau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>
#include <math.h>
#include "malloc.h"
#include <sys/mman.h>

t_page *pages = NULL;

#define page_size 4096*2


void split_block() {

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
			//ptr->blocks_nb++;
			printf("New block\n");
			pages = ptr;
			return (ptr->data + 1);
		}
	}
}
int		main(int argc, char **argv)
{
	printf("%d\n", getpagesize());
	char * str = (char *)malloc_test(10);
	memcpy(str, "0123456789", 10);

	return (0);
}
