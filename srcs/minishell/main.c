/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toliver <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/07 00:29:48 by toliver           #+#    #+#             */
/*   Updated: 2020/01/20 11:52:34 by toliver          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void			ft_sigtest(int signal)
{
	t_env		*env;

	env = ft_get_env();
	if (signal == SIGINT)
	{
		if (env->pid == 0)
			ft_printf("\n$>");
		else
			ft_printf("\n");
	}
	env->pid = 0;
}

void			ft_run1(t_env *env)
{
	ft_printf("$>");
	ft_set_env(env);
}

void			ft_run(t_env *env)
{
	int			retval;
	char		*str;

	ft_run1(env);
	signal(SIGINT, &ft_sigtest);
	while ((retval = ft_gnl(0, &str)) > 0)
	{
		if (str == NULL && !(str = ft_strdup("")))
		{
			retval = -1;
			break ;
		}
		if (!(ft_split_commands(env, str)))
			break ;
		if (!(ft_exec_commands(env)))
			break ;
		if (retval == 0)
		{
			free(str);
			ft_printf("\n");
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
	{
		ft_printf("%d\n", ac);
		return (ft_usage());
	}
	if (!(ft_env_init(&env, av, envp)))
		return (-1);
	ft_run(&env);
	ft_free_env(&env);
	return (0);
}
