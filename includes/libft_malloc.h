/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_malloc.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adoussau <adoussau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/20 22:45:53 by adoussau          #+#    #+#             */
/*   Updated: 2017/12/21 16:13:20 by adoussau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_MALLOC_H
# define LIBFT_MALLOC_H

# include <stdlib.h>
# include <unistd.h>
# include <math.h>

# define CEIL(v) ( ( v - (int)v ) == 0 ? (int)v : (int)v + 1 )
# define ARR(s,b) ( CEIL( ( s ) / b ) * b )

# define TINY_MAX_ALLOC 128
# define TINY_BLOCK_SIZE (TINY_MAX_ALLOC + sizeof(t_block))
# define TINY_SIZE ARR(TINY_BLOCK_SIZE * 100.0 + sizeof(t_page), getpagesize())
# define TINY_TYPE 1

# define SMALL_MAX_ALLOC 1024
# define SMALL_BLOCK_SIZE (SMALL_MAX_ALLOC + sizeof(t_block))
# define SMALL_SIZE ARR(SMALL_BLOCK_SIZE * 100. + sizeof(t_page), getpagesize())
# define SMALL_TYPE 2

# define LARGE_TYPE 3

typedef struct		s_block
{
	struct s_block	*next;
	struct s_block	*prev;
	size_t			size;
	size_t			state;
}					t_block;

typedef struct		s_page
{
	struct s_page	*next;
	struct s_block	*data;
	size_t			size;
	struct s_page	*prev;
	size_t			type;
}					t_page;

void				*malloc(size_t s);
void				*realloc(void *ptr, size_t size);
void				free(void *ptr);
void				show_alloc_mem(void);

void				malloc_putstr(const char *s);
void				malloc_putnbr(int nb);
void				malloc_printhex(unsigned char c);

void				print_block(t_block *b, char hide_free);
void				print_page(t_page *p, char hide_free);
void				print_pages(char hide_free);

t_block				*split_block(t_block *block, size_t size);
t_block				*allocate_block(t_block *block, size_t size);
t_block				*get_free_block(t_page *p, size_t s);
void				merge_free_block(t_block *b);
size_t				get_page_type(size_t s);

size_t				get_page_size(size_t s);
size_t				get_page_type(size_t s);
t_page				*allocate_new_page(size_t s);
void				free_page(t_page *page);
t_block				*search_block(void *ptr, t_page **page);

t_page				*g_pages;

#endif
