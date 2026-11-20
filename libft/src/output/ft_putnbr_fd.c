/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jose-cad <jose-cad@42sp.org.br>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 14:16:45 by jose-cad          #+#    #+#             */
/*   Updated: 2025/09/04 14:16:49 by jose-cad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	unsigned int	nbr;
	char			buf[11];
	int				i;

	if (n < 0)
		nbr = (unsigned int)(-(long)n);
	else
		nbr = n;
	i = 11;
	if (n == 0)
		buf[--i] = '0';
	while (nbr > 0)
	{
		buf[--i] = (nbr % 10) + '0';
		nbr /= 10;
	}
	if (n < 0)
		buf[--i] = '-';
	write(fd, buf + i, 11 - i);
}
