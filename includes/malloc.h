/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adoussau <adoussau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/20 22:45:53 by adoussau          #+#    #+#             */
/*   Updated: 2017/11/20 22:51:02 by adoussau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_H
# define MALLOC_H

typedef struct		s_block
{
	size_t			size;
	struct s_block	*prev;
	struct s_block	*next;
	int				state;
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
