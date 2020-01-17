/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toliver <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/17 09:28:57 by toliver           #+#    #+#             */
/*   Updated: 2020/01/17 09:30:45 by toliver          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int				ft_echo_internal(t_env *env)
{
	int			i;

	i = 1;
	while (env->command[i])
	{
		ft_printf("%s%c", env->command[i], env->command[i + 1] ? ' ' : '\0');
		i++;
	}
	ft_printf("\n");
	return (1);
}

int				ft_env_internal(t_env *env)
{
	ft_env(env);
	return (1);
}

int				ft_setenv_internal(t_env *env)
{
	int			i;
	char		*command;

	i = 1;
	while (env->command[i])
	{
		if (!(command = ft_strdup(env->command[i])))
			return (ft_crash(MALLOC_FAIL, NULL, env));
		if (!(ft_setenv(env, command)))
			return (0);
		i++;
	}
	return (1);
}

int				ft_unsetenv_internal(t_env *env)
{
	int			i;
	char		*command;

	i = 1;
	while (env->command[i])
	{
		if (!(command = ft_strdup(env->command[i])))
			return (ft_crash(MALLOC_FAIL, NULL, env));
		if (!(ft_unsetenv(env, command)))
		{
			free(command);
			return (0);
		}
		i++;
		free(command);
	}
	return (1);
}
