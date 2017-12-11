/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adoussau <adoussau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/03 18:01:29 by adoussau          #+#    #+#             */
/*   Updated: 2017/12/09 17:10:17 by adoussau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include "libft_malloc.h"

void		malloc_putstr(const char *s)
{
	while (*s)
		write(1, s++, 1);
}

static void	ft_putchar(char c)
{
	write(1, &c, 1);
}

void		malloc_putnbr(int nb)
{
	if (nb < 0)
	{
		ft_putchar('-');
		malloc_putnbr(-nb);
	}
	else if (nb < 10)
		ft_putchar(nb + '0');
	else
	{
		malloc_putnbr(nb / 10);
		ft_putchar(nb % 10 + '0');
	}
}

void		malloc_printhex(unsigned char c)
{
	unsigned char tmp;

	tmp = c >> 4;
	if (tmp < 10)
		ft_putchar(tmp + '0');
	else
		ft_putchar(tmp - 10 + 'A');
	tmp = c & 0x0F;
	if (tmp < 10)
		ft_putchar(tmp + '0');
	else
		ft_putchar(tmp - 10 + 'A');
}
