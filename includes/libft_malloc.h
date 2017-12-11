/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_malloc.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adoussau <adoussau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/20 22:45:53 by adoussau          #+#    #+#             */
/*   Updated: 2017/12/03 17:58:10 by adoussau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_MALLOC_H
# define LIBFT_MALLOC_H

# include <stdlib.h>
# include <unistd.h>

# define TINY_SIZE (2 * getpagesize())
# define TINY_MAX_ALLOC 128
# define TINY_TYPE 1

# define SMALL_SIZE (16 * getpagesize())
# define SMALL_MAX_ALLOC 1024
# define SMALL_TYPE 2

# define LARGE_TYPE 3

typedef struct		s_block
{
	struct s_block	*next;
	struct s_block	*prev;
	size_t			size;
	char			state;
}					t_block;

typedef struct		s_page
{
	struct s_page	*next;
	struct s_block	*data;
	size_t			size;
	struct s_page	*prev;
	char			type;
}					t_page;

void				*malloc(size_t s);
void				*realloc(void *ptr, size_t size);
void				free(void *ptr);
void				show_alloc_mem(void);

void				malloc_putstr(const char *s);
void				malloc_putnbr(int nb);
void				malloc_printhex(char c);

t_page				*g_pages;

#endif
