/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toliver <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/04 22:50:49 by toliver           #+#    #+#             */
/*   Updated: 2018/09/04 22:53:04 by toliver          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int				printenv(t_envs *env)
{
	t_var		*ptr;

	ptr = env->envp;
	while (ptr)
	{
		ft_printf("%s=%s\n", ptr->name, ptr->value);
		ptr = ptr->next;
	}
	return (1);
}

int				envshell(char **split, t_envs *env)
{
	printenv(env);
	(void)split;
	return (1);
}
