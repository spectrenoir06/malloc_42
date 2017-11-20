/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsimplefree.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adoussau <antoine@doussaud.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/16 16:27:00 by adoussau          #+#    #+#             */
/*   Updated: 2014/11/16 16:28:56 by adoussau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstsimplefree(t_list **alst)
{
	if (*alst)
	{
		ft_lstsimplefree(&(*alst)->next);
		ft_lstsimplefreeone(alst);
	}
}
