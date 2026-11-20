/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ftoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jose-cad <jose-cad@42sp.org.br>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 10:28:04 by jose-cad          #+#    #+#             */
/*   Updated: 2025/08/12 10:28:09 by jose-cad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

static long long	ft_power(long long nb, int power);
static char			*get_fraction_str(long long frac, int precision);
static char			*ft_join_parts(char *int_str, char *frac_str, int is_neg);
static char			*ft_lltoa_pos(long long n);

char	*ft_ftoa(double n, int precision)
{
	long long	int_part;
	long long	frac_part;
	long long	mult;
	int			is_neg;

	if (precision < 0)
		precision = 6;
	is_neg = (n < 0);
	if (n < 0)
		n = -n;
	mult = ft_power(10, precision);
	int_part = (long long)n;
	frac_part = (long long)((n - int_part) * mult + 0.5);
	if (frac_part >= mult)
	{
		frac_part -= mult;
		int_part++;
	}
	return (ft_join_parts(ft_lltoa_pos(int_part),
			get_fraction_str(frac_part, precision), is_neg));
}

static char	*get_fraction_str(long long frac, int precision)
{
	char	*str;
	int		i;

	str = malloc(precision + 1);
	if (!str)
		return (NULL);
	str[precision] = '\0';
	i = precision - 1;
	while (i >= 0)
	{
		str[i] = (frac % 10) + '0';
		frac /= 10;
		i--;
	}
	return (str);
}

static char	*ft_join_parts(char *int_str, char *frac_str, int is_neg)
{
	char	*temp;
	char	*res;

	if (!int_str || !frac_str)
		return (free(int_str), free(frac_str), NULL);
	if (frac_str[0] == '\0')
		res = int_str;
	else
	{
		temp = ft_strjoin(".", frac_str);
		if (!temp)
			return (free(int_str), free(frac_str), NULL);
		res = ft_strjoin(int_str, temp);
		free(int_str);
		free(temp);
	}
	free(frac_str);
	if (res && is_neg)
	{
		temp = ft_strjoin("-", res);
		free(res);
		return (temp);
	}
	return (res);
}

static char	*ft_lltoa_pos(long long n)
{
	char	buf[21];
	int		i;

	if (n == 0)
		return (ft_strdup("0"));
	i = 20;
	buf[i] = '\0';
	while (n > 0)
	{
		i--;
		buf[i] = (n % 10) + '0';
		n /= 10;
	}
	return (ft_strdup(&buf[i]));
}

static long long	ft_power(long long nb, int power)
{
	long long	result;

	result = 1;
	if (power < 0)
		return (0);
	if (nb == 0 && power == 0)
		return (1);
	while (power > 0)
	{
		result *= nb;
		power--;
	}
	return (result);
}
