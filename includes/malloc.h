/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adoussau <adoussau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/20 22:45:53 by adoussau          #+#    #+#             */
/*   Updated: 2017/11/27 00:27:34 by adoussau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#ifndef MALLOC_H
# define MALLOC_H

typedef struct		s_block
{
	// char			magic;
	size_t			size;
	struct s_block	*prev;
	struct s_block	*next;
	int				state;
	// char			magic2;
}					t_block;

typedef struct		s_page
{
	int				type;
	size_t			size;
	struct s_page	*prev;
	struct s_page	*next;
	struct s_block	*data;
	int				blocks_nb;
}					t_page;


#endif
