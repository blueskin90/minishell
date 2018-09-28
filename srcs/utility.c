/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utility.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toliver <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/04 21:19:15 by toliver           #+#    #+#             */
/*   Updated: 2018/09/28 17:50:07 by toliver          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int				printvarvalue(char *str, t_var *list)
{
	t_var		*ptr;

	ptr = list;
	while (ptr)
	{
		if (ft_strcmp(ptr->name, str) == 0)
			ft_putstr(ptr->value);
		ptr = ptr->next;
	}
	return (0);
}

char			*getvarvalue(char *str, t_var *list)
{
	t_var		*ptr;

	ptr = list;
	while (ptr)
	{
		if (ft_strcmp(ptr->name, str) == 0)
			return (ptr->value);
		ptr = ptr->next;
	}
	return (NULL);
}

int				isquote(char c)
{
	if (c == '\'' || c == '\"')
		return (1);
	return (0);
}

int				iswhitespace(char c)
{
	if (c == '\t' || c == '\n' || c == '\r' || c == '\v' || c == '\f'
			|| c == ' ')
		return (1);
	return (0);
}

int				returnval(int val, t_envs *env)
{
	env->running = val;
	return (val);
}
