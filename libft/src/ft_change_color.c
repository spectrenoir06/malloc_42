/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_color.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adoussau <antoine@doussaud.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/07 17:24:42 by adoussau          #+#    #+#             */
/*   Updated: 2014/11/07 17:29:22 by adoussau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

typedef enum	e_color
{
	BLACK = 0,
	RED = 1,
	GREEN = 2,
	YELLOW = 3,
	BLUE = 4,
	MANGENTA = 5,
	CYAN = 6,
	WHITE = 7,
	DEFAULT = 9
}				t_color;

void	ft_fg(t_color color)
{
	ft_putstr("\E[");
	ft_putnbr(color + 30);
	ft_putchar('m');
}

void	ft_bg(t_color color)
{
	ft_putstr("\E[");
	ft_putnbr(color + 40);
	ft_putchar('m');
}

void	ft_color(t_color fg, t_color bg)
{
	ft_bg(fg);
	ft_fg(bg);
}

void

void	ft_clear()
{
	ft_putstr("\E[H\E2");
}

int main()
{
	ft_fg(YELLOW);
	write(1, "hello\n", 6);
	ft_bg(GREEN);
	write(1, "hello\n", 6);
	ft_color(YELLOW, BLUE);
	write(1, "hello\n", 6);
	return (0);
}
