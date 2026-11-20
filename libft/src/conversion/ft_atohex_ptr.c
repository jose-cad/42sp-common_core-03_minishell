/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atohex_ptr.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jose-cad <jose-cad@42sp.org.br>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 10:37:32 by jose-cad          #+#    #+#             */
/*   Updated: 2025/08/12 10:37:38 by jose-cad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_hex_digit(char c);

unsigned long	ft_atohex_ptr(const char *hex_str)
{
	unsigned long	result;

	if (!hex_str)
		return (0);
	while (ft_isspace(*hex_str))
		hex_str++;
	if (*hex_str == '-')
		return (0);
	if (*hex_str == '+')
		hex_str++;
	result = 0;
	while (ft_hex_digit(*hex_str) != -1)
	{
		result = result * 16 + ft_hex_digit(*hex_str);
		hex_str++;
	}
	return (result);
}

static int	ft_hex_digit(char c)
{
	c = ft_tolower((unsigned char)c);
	if (c >= '0' && c <= '9')
		return (c - '0');
	if (c >= 'a' && c <= 'f')
		return (c - 'a' + 10);
	return (-1);
}
