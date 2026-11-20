/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoul_base.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jose-cad <jose-cad@42sp.org.br>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/14 23:52:00 by jose-cad          #+#    #+#             */
/*   Updated: 2026/11/14 23:52:00 by jose-cad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_base_index(char c, const char *base);

unsigned long	ft_atoul_base(const char *str, const char *base)
{
	unsigned long	result;
	int				sign;
	int				base_len;

	result = 0;
	sign = 1;
	if (!str || !ft_base_is_valid(base))
		return (0);
	base_len = ft_strlen(base);
	while (ft_isspace(*str))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str && ft_base_index(*str, base) != -1)
	{
		result = result * base_len + ft_base_index(*str, base);
		str++;
	}
	return (result * sign);
}

static int	ft_base_index(char c, const char *base)
{
	int	i;

	i = 0;
	while (base[i])
	{
		if (c == base[i])
			return (i);
		i++;
	}
	return (-1);
}
