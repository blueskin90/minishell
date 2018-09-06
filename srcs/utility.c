/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utility.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toliver <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/04 21:19:15 by toliver           #+#    #+#             */
/*   Updated: 2018/09/05 16:30:44 by toliver          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char			*capitalize(char *str)
{
	int			i;

	i = 0;
	while (str[i])
	{
		if (str[i] >= 'a' && str[i] <= 'z')
			str[i] -= 32;
		i++;
	}
	return (str);
}

int				incrementshlevel(t_envs *env)
{
	t_var		*ptr;
	char		*value;

	ptr = env->envp;
	while (ptr && ft_strcmp(ptr->name, "SHLVL"))
		ptr = ptr->next;
	if (ptr)
	{
		if (!(value = ft_itoa(ft_atoi(ptr->value) + 1)))
			env->running = -1;
		free(ptr->value);
		ptr->value = value;
	}
	else
		addenvvar("SHLVL", "1", env);
	return (1);
}

int				pwdexist(t_var *list)
{
	t_var		*ptr;

	ptr = list;
	while (ptr)
	{
		if (ft_strcmp(ptr->name, "PWD") == 0)
			return (1);
		ptr = ptr->next;
	}
	return (0);
}

int				envvarexist(char *str, t_var *list)
{
	t_var		*ptr;

	ptr = list;
	while (ptr)
	{
		if (ft_strcmp(ptr->name, str) == 0)
			return (1);
		ptr = ptr->next;
	}
	return (0);
}

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
			return(ptr->value);
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
