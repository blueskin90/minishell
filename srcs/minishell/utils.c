/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toliver <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/12 01:54:46 by toliver           #+#    #+#             */
/*   Updated: 2020/01/17 08:41:01 by toliver          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int				ft_is_charset(char c, char *charset)
{
	while (*charset)
	{
		if (c == *charset)
			return (1);
		charset++;
	}
	return (0);
}

char			**ft_split_charset_allocarray(char *str, char *charset)
{
	int			nbr;
	char		**array;

	nbr = 0;
	while (str && *str)
	{
		if (!ft_is_charset(*str, charset))
		{
			nbr++;
			while (*str && !ft_is_charset(*str, charset))
				str++;
		}
		while (*str && ft_is_charset(*str, charset))
			str++;
	}
	array = (char**)malloc(sizeof(char*) * (nbr + 1));
	ft_bzero(array, sizeof(char*) * (nbr + 1));
	return (array);
}

int				ft_split_charset_allocstr(char *str, char *charset, char **arr)
{
	int			wrd;
	int			i;

	wrd = 0;
	while (str && *str)
	{
		if (!ft_is_charset(*str, charset))
		{
			i = 0;
			while (str[i] && !ft_is_charset(str[i], charset))
				i++;
			if (!(arr[wrd] = (char*)malloc(sizeof(char) * (i + 1))))
				return (0);
			ft_strncpy(arr[wrd], str, i);
			arr[wrd][i] = '\0';
			str += i;
			wrd++;
		}
		while (*str && ft_is_charset(*str, charset))
			str++;
	}
	return (1);
}

char			**ft_split_free(char **array)
{
	int			i;

	i = 0;
	while (array && array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
	return (NULL);
}

char			**ft_split_charset(char *str, char *charset)
{
	char		**array;

	if (!(array = ft_split_charset_allocarray(str, charset)))
		return (NULL);
	if (!ft_split_charset_allocstr(str, charset, array))
		return (ft_split_free(array));
	return (array);
}
