/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toliver <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/12 01:50:21 by toliver           #+#    #+#             */
/*   Updated: 2020/01/17 08:48:00 by toliver          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int				ft_exec_env(t_env *env, char **envp, char **commands)
{
	t_env		envcpy;

	ft_memcpy(&envcpy, env, sizeof(t_env));
	envcpy.command = commands;
	envcpy.envp.env = envp;
	envcpy.envp.occupied = ft_env_getsize(envp);
	envcpy.envp.size = ft_env_getsize(envp) + 1;
	return (ft_exec_command(&envcpy));
}

int				ft_env(t_env *env)
{
	char		**tmp_env;
	int			i;

	i = 1;
	if ((!env->command[1] || !ft_strequ(env->command[1], "-i")) &&
			!(tmp_env = ft_env_cpy(env)))
		return (ft_crash(MALLOC_FAIL, NULL, env));
	if (ft_strequ(env->command[1], "-i") && i++ &&
			!(tmp_env = (char**)ft_memalloc(sizeof(char*))))
		return (ft_crash(MALLOC_FAIL, NULL, env));
	if (!(ft_env_add_params(&tmp_env, env, &i)))
	{
		ft_env_cpy_free(tmp_env);
		return (ft_crash(MALLOC_FAIL, NULL, env));
	}
	if (env->command[i] == NULL)
		ft_dump_env(tmp_env);
	else
	{
		i = ft_exec_env(env, tmp_env, &env->command[i]);
		ft_env_cpy_free(tmp_env);
		return (i);
	}
	ft_env_cpy_free(tmp_env);
	return (1);
}
