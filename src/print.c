/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adoussau <adoussau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/03 18:01:29 by adoussau          #+#    #+#             */
/*   Updated: 2017/12/03 19:30:54 by adoussau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>

void	putstr(const char *s)
{
	while (*s)
		write(1, s++, 1);
}

static void	ft_putchar(char c)
{
	write(1, &c, 1);
}

void	putnbr(int nb)
{
	if (nb < 0)
	{
		ft_putchar('-');
		putnbr(-nb);
	}
	else if (nb < 10)
		ft_putchar(nb + '0');
	else
	{
		putnbr(nb / 10);
		ft_putchar(nb % 10 + '0');
	}
}

void printhex(unsigned char c) {
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
