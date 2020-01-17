/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toliver <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/17 08:34:00 by toliver           #+#    #+#             */
/*   Updated: 2020/01/17 08:36:23 by toliver          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int				ft_envp_atoi(t_env *env, char *value)
{
	size_t		num;

	num = 0;
	while (value && *value && ft_is_charset(*value, "\t\n\r\v\f "))
		value++;
	if (value && *value && *value == '-')
		return (-1);
	if (value && *value && *value == '+')
		value++;
	while (value && *value && ft_is_charset(*value, "0123456789"))
	{
		num *= 10;
		num += (*value - '0');
		value++;
		if (num > INT_MAX)
		{
			ft_dprintf(2, "%s: warning: shell level (%zu) too high, %s",
					env->prog_name, value, "resetting to 1\n");
			return (0);
		}
	}
	return (num);
}

int				ft_envp_shlevel(t_env *env)
{
	char		*value;
	int			realval;
	char		*itoaed;

	value = ft_env_get_value(env, "SHLVL");
	if (!value)
		realval = 0;
	else
		realval = ft_envp_atoi(env, value);
	realval += 1;
	if (!(itoaed = ft_itoa(realval)))
		return (ft_crash(MALLOC_FAIL, NULL, env));
	if (!(value = ft_envp_concat("SHLVL", itoaed)))
	{
		free(itoaed);
		return (ft_crash(MALLOC_FAIL, NULL, env));
	}
	free(itoaed);
	ft_setenv(env, value);
	return (1);
}

int				ft_envp_getpwd(t_env *env)
{
	char		*currpwd;
	char		*concatenated;

	if (!(currpwd = getcwd(NULL, 0)))
		return (ft_crash(MISSING_CWD, NULL, env));
	if (!(concatenated = ft_envp_concat("PWD", currpwd)))
	{
		free(currpwd);
		return (ft_crash(MALLOC_FAIL, NULL, env));
	}
	free(currpwd);
	ft_setenv(env, concatenated);
	return (1);
}

int				ft_envp_set_(t_env *env)
{
	int			i;
	char		*concat;

	i = 0;
	while (env->command && env->command[i])
		i++;
	i--;
	if (!(concat = ft_envp_concat("_", env->command[i])))
		return (0);
	return (ft_setenv(env, concat));
}

int				ft_envp_get_(t_env *env)
{
	char		*concat;

	if (!(concat = ft_envp_concat("_", env->prog_name)))
		return (ft_crash(MALLOC_FAIL, NULL, env));
	return (ft_setenv(env, concat));
}
