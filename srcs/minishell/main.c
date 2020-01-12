/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toliver <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/07 00:29:48 by toliver           #+#    #+#             */
/*   Updated: 2020/01/12 04:28:22 by toliver          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int				ft_echo_internal(t_env *env)
{
	(void)env;
	ft_printf("echo !\n");
	return (1);
}

int				ft_env_internal(t_env *env)
{
	ft_env(env);
	return (1);
}

int				ft_setenv_internal(t_env *env)
{
	(void)env;
	ft_printf("setenv !\n");
	return (1);
}

int				ft_unsetenv_internal(t_env *env)
{
	(void)env;
	ft_printf("unsetenv !\n");
	return (1);
}

int				ft_cd_internal(t_env *env)
{
	(void)env;
	ft_printf("cd !\n");
	return (1);
}

void			ft_run(t_env *env)
{
	int			retval;
	char		*str;

	ft_printf("$>");
	while ((retval = ft_gnl(0, &str)) > -1)
	{
		if (ft_strnequ("env", str, 3))
			ft_env_internal(env);
		if (ft_strnequ("unsetenv", str, 8))
			ft_unsetenv_internal(env);
		if (ft_strnequ("setenv", str, 6))
			ft_setenv_internal(env);
		if (ft_strnequ("echo", str, 4))
			ft_echo_internal(env);
		if (ft_strnequ("cd", str, 4))
			ft_cd_internal(env);
		if (ft_strnequ("exit", str, 4))
		{
			free(str);
			break;
		}
		ft_printf("$>");
	}
	if (retval == -1)
		ft_crash(MALLOC_FAIL, NULL, env);
}

int				main(int ac, char **av, char **envp)
{
	t_env		env;

	if (ac != 1)
		return (ft_usage());
	if (!(ft_env_init(&env, av, envp)))
		return (-1);
	ft_run(&env);
	ft_free_env(&env);
	return (0);
}
