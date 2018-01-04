/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_mem.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adoussau <adoussau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/03 18:01:29 by adoussau          #+#    #+#             */
/*   Updated: 2018/01/04 16:01:52 by adoussau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_malloc.h"

void	malloc_putaddr(unsigned long ptr)
{
	int i;
	char *ptr2 = (char *)&ptr;

	malloc_putstr("0x");
	i = 5;
	while (i >= 0)
		malloc_printhex(ptr2[i--]);
}

void	print_block(t_block *b, char hide_free)
{
	int size;
	int i;

	if (hide_free && !b->state)
		return ;
	malloc_putstr("\t\t----------Block-----------\n\t\tSize:  ");
	malloc_putnbr(b->size);
	malloc_putstr("  (real: ");
	malloc_putnbr(b->size + sizeof(t_block));
	malloc_putstr(")\n\t\tState: ");
	malloc_putnbr(b->state);
	malloc_putstr("\n\t\tAdresse:  ");
	malloc_putaddr((unsigned long)(b + 1));
	malloc_putstr("\n\t\tData:  ");
	size = b->size < 25 ? b->size : 25;
	i = 0;
	while (i < size)
	{
		malloc_printhex(*(((unsigned char *)(b + 1)) + i++));
		write(1, " ", 1);
	}
	write(1, "\n", 1);
}

void	print_page(t_page *p, char hide_free)
{
	t_block *b;

	malloc_putstr("\t------------Page------------\n\tSize: ");
	malloc_putnbr(p->size);
	malloc_putstr("  (real: ");
	malloc_putnbr(p->size + sizeof(t_page));
	malloc_putstr(")\n\ttype: ");
	if (p->type == 1)
		malloc_putstr("TINY\n");
	if (p->type == 2)
		malloc_putstr("SMALL\n");
	if (p->type == 3)
		malloc_putstr("LARGE\n");
	b = p->data;
	while (b)
	{
		print_block(b, hide_free);
		b = b->next;
	}
}

void	print_pages(char hide_free)
{
	t_page *p;

	p = g_pages;
	malloc_putstr("\n---- PRINT PAGE ----\n");
	while (p)
	{
		print_page(p, hide_free);
		p = p->next;
	}
}

void	show_alloc_mem(void)
{
	print_pages(1);
}
