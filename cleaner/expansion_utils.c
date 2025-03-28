/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimatayi <dimatayi@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 23:10:10 by dimatayi          #+#    #+#             */
/*   Updated: 2025/03/18 02:03:26 by dimatayi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	ft_len3(char *s)
{
	int	i;

	i = 0;
	if (s[0] == 34)
		i++;
	while (s[i] && s[i] != 34)
		i++;
	return (++i);
}

int	ft_len2(char *s)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	if (s[0] == 34)
		i++;
	while (s[i] && s[i] != 34)
		i++;
	if (s[i] == 34)
		i++;
	while (s[i + j])
		j++;
	return (j);
}

int	ft_len(char *s, char c)
{
	int	i;

	i = 0;
	while (s[i] && s[i] != c)
		i++;
	return (i);
}

int	is_single_quote(char *s, int dollar_index)
{
	int	i;
	int	counter;

	i = 0;
	counter = 0;
	while (s[i] && i < dollar_index)
	{
		if (s[i] == 39)
			counter++;
		i++;
	}
	if (counter == 0 || counter % 2 == 0)
		return (0);
	return (1);
}