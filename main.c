#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libft_malloc.h>

int		main(int argc, char **argv)
{
	(void)argc;
	(void)argv;

	show_alloc_mem();

	char* test[25];

	for (int i=0;i<10;i++) {
		test[i] = (char*)malloc(10);
		memset(test[i], 0x42, 10);
	}

	for (int i=0;i<10;i++) {
		test[10+i] = (char*)malloc(TINY_MAX_ALLOC+1);
		memset(test[10+i], 0x42, TINY_MAX_ALLOC+1);
	}

	for (int i=0;i<5;i++) {
		test[20+i] = (char*)malloc(SMALL_MAX_ALLOC+1);
		memset(test[20+i], 0x42, SMALL_MAX_ALLOC+1);
	}

	print_pages(0);

	for (int i=0;i<25;i++) {
		free(test[i]);
	}

	print_pages(0);

	char *test2 = (char *)malloc(10);
	memset(test2, 0x42, 10);
	char *dummy = (char *)malloc(1);
	print_pages(0);

	for (int i=0; i<10; i++) {
		test2 = (char *)realloc(test2, 11+i);
	}
	print_pages(0);

	test2 = (char *)realloc(test2, 10000);
	print_pages(0);

	free(dummy);
	print_pages(0);

	free(test2);
	print_pages(0);

	return (0);
}
