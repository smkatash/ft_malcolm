/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanykei <kanykei@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/28 14:41:10 by ktashbae          #+#    #+#             */
/*   Updated: 2024/04/11 11:48:17 by kanykei          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_isspace(char *p)
{
	int	i;

	i = 0;
	while (p[i] == ' ' || p[i] == '\n' || p[i] == '\t' || p[i] == '\v'
		|| p[i] == '\f' || p[i] == '\r')
	{
		i++;
	}
	return (p + i);
}

int	ft_check(char *s, int i)
{
	int	neg;

	neg = 1;
	if (s[i] == '-')
	{
		neg *= -1;
	}
	return (neg);
}

int	ft_atoi(const char *str)
{
	int		i;
	long	num;
	int		neg;
	char	*p;

	i = 0;
	neg = 1;
	num = 0;
	p = (char *)str;
	p = ft_isspace(p);
	if (p[i] == '-' || p[i] == '+')
	{
		neg = ft_check(p, i);
		i++;
	}
	while (p[i] >= '0' && p[i] <= '9')
	{
		num = (num * 10) + (p[i] - '0');
		if (num > 2147483648 && neg == -1)
			return (0);
		if (num > 2147483647 && neg == 1)
			return (-1);
		i++;
	}
	return (num * neg);
}