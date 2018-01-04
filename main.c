#include <stdio.h>
#include <stdlib.h>
#include <libft_malloc.h>


int		main(int argc, char **argv)
{
	(void)argc;
	(void)argv;
	// printf("getpagesize() = %d\n", getpagesize());

	// printf("g_pages = %p\n", g_pages);
	//
	//
	// printf("sizeof(t_page)=%ld\n" , sizeof(t_page));
	// printf("sizeof(t_block)=%ld\n", sizeof(t_block));

	// show_alloc_mem();

	// char* tab[100];
	//
	// for (int i=0;i<10;i++) {
	// 	tab[i] = (char*)malloc(10);
	// }
	// for (int i=0; i<10; i++) {
	// 	for (int j=0; j<10; j++)
	// 		tab[i][j] = ('a'+ i);
	// 	tab[i][10] = 0;
	// }

	// for (int i=0;i<5;i++) {
	// 	tab[i+5] = (char*)malloc(100);
	// }
	// for (int i=0; i<5; i++) {
	// 	for (int j=0; j<10; j++)
	// 		tab[i+5][j] = ('a'+ i);
	// 	tab[i+5][10] = 0;
	// }



	// malloc(500);

	char *tab[5];

	for (int i=0; i < 5; i++) {
		tab[i] = NULL;
	}


	tab[0] = malloc(10);

	memcpy(tab[0], "01234567\xAA\0", 10);

	print_pages(0);

	tab[1] = malloc(10);

	memcpy(tab[1], "999999999\0", 10);
	// tab[2] = malloc(10);
	// tab[3] = malloc(10);
	// tab[4] = malloc(10);
	//
	// memcpy(tab[0], "01234567\xAA\0", 10);
	//
	tab[2] = realloc(tab[0], 20000);
	// memcpy(tab[2], "abcdeifjk\0", 10);

	print_pages(0);

	for (int i=0; i < 5; i++) {
		printf("tab[%d] = %p\n", i,tab[i]);
	}

	// strdup("test");

	//printf("malloc(10) == %p\n", malloc(10));
	// printf("malloc(10) == %p\n", malloc(10));
	// printf("malloc(10) == %p\n", malloc(10));
	// printf("malloc(10) == %p\n", malloc(10));




	// print_pages(0);


	// printf("realloc(str, 20) == %p\n", realloc(str, 20));
	//
	// for (int i=0; i < 10; i++) {
	// 	realloc(str, i*10);
	// 	print_page(pages);
	// }

	// memcpy(str,"0123456789012345678\0", 20);
	//
	// free(tab[0]);
	// free(tab[1]);
	// free(tab[2]);
	// free(tab[3]);
	// free(tab[4]);
	//

	// free(tab[0+5]);
	// free(tab[1+5]);
	// free(tab[2+5]);
	// free(tab[3+5]);
	// free(tab[4+5]);


	// malloc(10);
	// malloc(10);
	// malloc(10);
	//



	// memcpy(str2, "1111111111111111", 16);
	// memcpy(str3, "0123456789012345", 16);
	// memcpy(str4, "0123456789012345", 16);

	// memcpy(str, "0123456789", 10);

	return (0);
}
