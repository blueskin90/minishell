/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toliver <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/05 01:57:56 by toliver           #+#    #+#             */
/*   Updated: 2018/09/05 01:58:31 by toliver          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int				pwdshell(char **splittedline, t_envs *env)
{
	t_var		*ptr;

	if (splittedline[0])
		ft_printf("pwd: too many arguments\n");
	else
	{
		ptr = env->envp;
		while (ptr && ft_strcmp(ptr->name, "PWD") != 0)
			ptr = ptr->next;
		if (ptr)
			ft_printf("%s\n", ptr->value);
		else
			ft_printf("env variable: $PWD is not set\n");
	}
	return (1);
}
