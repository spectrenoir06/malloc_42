
#include <libft_malloc.h>

int main()
{
	printf("%d\n", TINY_SIZE);
	char* str = malloc(128);
	strcpy(str, "Hello");
	malloc(1024);
	malloc(1024 * 32);
	malloc(1024 * 1024);
	malloc(1024 * 1024 * 16);
	malloc(1024 * 1024 * 128);
	show_alloc_mem();
	return (0);
}
