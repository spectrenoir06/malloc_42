/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adoussau <adoussau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/20 22:45:53 by adoussau          #+#    #+#             */
/*   Updated: 2017/11/29 22:35:31 by adoussau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#ifndef MALLOC_H
# define MALLOC_H

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
	// struct s_page	*prev;
	unsigned short	blocks_nb;
	char			type;
}					t_page;


#endif
