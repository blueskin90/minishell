/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unsetenv.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toliver <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/05 01:57:30 by toliver           #+#    #+#             */
/*   Updated: 2018/09/12 20:05:34 by toliver          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int				freenode(t_var *ptr)
{
	free(ptr->name);
	free(ptr->value);
	free(ptr);
	return (1);
}

int				delnode(t_var *todel, t_var **list)
{
	t_var		*ptr;
	t_var		*tmp;

	if (*list == todel)
	{
		tmp = *list;
		*list = (*list)->next;
		freenode(tmp);
	}
	else
	{
		ptr = *list;
		while (ptr && ptr->next != todel)
			ptr = ptr->next;
		if (ptr->next == todel)
		{
			tmp = ptr->next;
			ptr->next = tmp->next;
			freenode(tmp);
		}
	}
	return (1);
}

int				unsetenvshell(char **split, t_envs *env)
{
	t_var		*ptr;
	int			i;

	i = 0;
	ptr = env->envp;
	while (ptr && split[i])
	{
		if (ft_strcmp(ptr->name, split[i]) == 0)
		{
			delnode(ptr, &env->envp);
			ptr = env->envp;
		}
		ptr = ptr->next;
		if (ptr == NULL && split[i])
		{
			i++;
			ptr = env->envp;
		}
	}
	return (1);
}
