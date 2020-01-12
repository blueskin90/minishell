/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toliver <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/07 00:29:48 by toliver           #+#    #+#             */
/*   Updated: 2020/01/12 07:08:36 by toliver          ###   ########.fr       */
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

int				ft_expand_command(t_env *env)
{
	(void)env;
	// ici expand toutes les $ et les ~ contenus dans les commandes
	return (1);
}

int				ft_split_command(t_env *env, char *str)
{
	ft_free_command(env);
	if (!(env->command = ft_split(str, "\t\n\r\v\f ")))
	{
		free(str);
		return (ft_crash(MALLOC_FAIL, NULL, env));
	}
	free(str);
	return (1);
}

int				ft_exec(t_env *env)
{
	(void)env;
	ft_printf("exec !\n");
	return (1);
}

int				ft_exec_command(t_env *env)
{
	char		*str;

	str = env->command[0];
	if (str == NULL)
		return (1);
	if (ft_strnequ("env", str, 3))
		return (ft_env_internal(env));
	if (ft_strnequ("unsetenv", str, 8))
		return (ft_unsetenv_internal(env));
	if (ft_strnequ("setenv", str, 6))
		return (ft_setenv_internal(env));
	if (ft_strnequ("echo", str, 4))
		return (ft_echo_internal(env));
	if (ft_strnequ("cd", str, 4))
		return (ft_cd_internal(env));
	if (ft_strnequ("exit", str, 4))
		return (0);
	return (ft_exec(env));
}

void			ft_run(t_env *env)
{
	int			retval;
	char		*str;

	ft_printf("$>");
	while ((retval = ft_gnl(0, &str)) > -1)
	{
		if (!(ft_split_command(env, str)))
			break;
		if (!(ft_expand_command(env)))
			break;
		if (!(ft_exec_command(env)))
			break;
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
