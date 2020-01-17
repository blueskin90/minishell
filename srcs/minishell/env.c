/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toliver <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/12 01:50:21 by toliver           #+#    #+#             */
/*   Updated: 2020/01/17 13:19:12 by toliver          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int		ft_exec_command_env(t_env *env)
{
	char		*str;

	str = env->command[0];
	if (str == NULL)
		return (1);
	if (!str[0])
		return (1);
	if (ft_strnequ("env", str, 3) && !str[3])
		return (ft_env_internal(env));
	if (ft_strnequ("unsetenv", str, 8) && !str[8])
		return (ft_unsetenv_internal(env));
	if (ft_strnequ("setenv", str, 6) && !str[6])
		return (ft_setenv_internal(env));
	if (ft_strnequ("echo", str, 4) && !str[4])
		return (ft_echo_internal(env));
	if (ft_strnequ("cd", str, 2) && !str[2])
		return (ft_cd_internal(env));
	if (ft_strnequ("exit", str, 4) && !str[4])
	{
		ft_printf("exit\n");
		return (0);
	}
	return (ft_exec(env));
}

int				ft_exec_env(t_env *env, char **envp, char **commands)
{
	t_env		envcpy;

	ft_memcpy(&envcpy, env, sizeof(t_env));
	envcpy.command = commands;
	envcpy.envp.env = envp;
	envcpy.envp.occupied = ft_env_getsize(envp);
	envcpy.envp.size = ft_env_getsize(envp) + 1;
	return (ft_exec_command_env(&envcpy));
}

// crash sur le env -i
int				ft_env(t_env *env)
{
	char		**tmp_env;
	int			i;

	i = 1;
	if ((!env->command[1] || !ft_strequ(env->command[1], "-i")) &&
			!(tmp_env = ft_env_cpy(env)))
		return (ft_crash(MALLOC_FAIL, NULL, env));
	if (ft_strequ(env->command[1], "-i") && ++i &&
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
		i = ft_exec_env(env, tmp_env, env->command + i);
		ft_env_cpy_free(tmp_env);
		return (i);
	}
	ft_env_cpy_free(tmp_env);
	return (1);
}
