/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adoussau <adoussau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/04 14:30:57 by adoussau          #+#    #+#             */
/*   Updated: 2017/11/21 03:28:02 by adoussau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	char	*dst;
	size_t	l;

	l = ft_strlen(s1);
	dst = (char *)malloc(sizeof(char) * l + sizeof(char));
	if (!dst)
		return (NULL);
	return (ft_strcpy(dst, s1));
}
