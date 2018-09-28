/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utility2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toliver <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/28 17:49:55 by toliver           #+#    #+#             */
/*   Updated: 2018/09/28 17:50:06 by toliver          ###   ########.fr       */
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
