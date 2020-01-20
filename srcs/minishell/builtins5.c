/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins5.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toliver <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/17 09:41:04 by toliver           #+#    #+#             */
/*   Updated: 2020/01/20 11:08:45 by toliver          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char			**ft_split_variables_free(char **tab)
{
	int			i;

	i = 0;
	while (tab && tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
	return (NULL);
}

int				ft_split_valloc_copy(char **tab, int *num, int *i, char **str)
{
	if (!(tab[*num] = (char*)malloc(sizeof(char) * (*i + 1))))
	{
		ft_split_variables_free(tab);
		return (0);
	}
	ft_strncpy(tab[*num], *str, *i);
	tab[*num][*i] = '\0';
	(*num)++;
	(*str) += *i;
	*i = 0;
	return (1);
}

char			**ft_split_variables_alloc(char *str, int count)
{
	char		**tab;
	int			num;
	int			i;

	i = 0;
	if (!(tab = (char**)ft_memalloc(sizeof(char*) * (count + 1))))
		return (NULL);
	num = 0;
	while (str && str[i])
	{
		while (str[i] && str[i] != '$')
			i++;
		if (i != 0 && !(ft_split_valloc_copy(tab, &num, &i, &str)))
			return (NULL);
		if (str[i] == '$' && ++i)
		{
			while (str[i] && !ft_is_charset(str[i], "\t\n\r\v\f $"))
				i++;
			if (!(ft_split_valloc_copy(tab, &num, &i, &str)))
				return (NULL);
		}
	}
	return (tab);
}

char			**ft_split_variables(char *str)
{
	int			i;
	int			count;

	i = 0;
	count = 1;
	while (str && str[i])
	{
		if (str[i] == '$')
		{
			count++;
			i++;
			while (str[i] && !ft_is_charset(str[i], "\t\n\r\v\f $"))
				i++;
			if (str[i])
				count++;
		}
		else
			i++;
	}
	return (ft_split_variables_alloc(str, count));
}

int				ft_expand_replace_variable(t_env *env, char **tab)
{
	int			i;
	char		*value;

	i = 0;
	while (tab && tab[i])
	{
		if (tab[i][0] == '$')
		{
			value = ft_env_get_value(env, tab[i] + 1);
			if (value == NULL)
			{
				if (!(value = ft_strdup("")))
					return (0);
			}
			else if (!(value = ft_strdup(value)))
				return (0);
			free(tab[i]);
			tab[i] = value;
		}
		i++;
	}
	return (1);
}
