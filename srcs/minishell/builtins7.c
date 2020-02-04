/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins7.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toliver <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/25 17:02:25 by toliver           #+#    #+#             */
/*   Updated: 2020/01/25 17:03:39 by toliver          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int				ft_cd_internal(t_env *env)
{
	if (env->command[1] == NULL)
		return (ft_cd_home(env));
	else if (env->command[1] && env->command[1][0] == '-'
			&& env->command[1][1] == '\0')
		return (ft_cd_minus_internal(env));
	else
		return (ft_cd(env, env->command[1]));
	return (1);
}

int				ft_exec_command(t_env *env)
{
	char		*str;

	str = env->command[0];
	if (str == NULL)
		return (1);
	if (!str[0])
		return (1);
	if (!(ft_envp_set_(env)))
		return (0);
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
