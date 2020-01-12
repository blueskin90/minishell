/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toliver <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/12 01:48:16 by toliver           #+#    #+#             */
/*   Updated: 2020/01/12 18:50:20 by toliver          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int				ft_envp_realloc(t_env *env, int size)
{
	char		**tmp;
	int			i;

	if (!(tmp = (char**)malloc(sizeof(char*) * (size + 1))))
		return (ft_crash(MALLOC_FAIL, NULL, env));
	i = 0;
	ft_bzero(tmp, sizeof(char*) * (size + 1));
	while (i < env->envp.occupied)
	{
		tmp[i] = env->envp.env[i];
		i++;
	}
	env->envp.size = size;
	free(env->envp.env);
	env->envp.env = tmp;
	return (1);
}

char			*ft_envp_concat(char *str1, char *str2)
{
	char		*str;
	int			size;

	size = 1;
	size += ft_strlen(str1);
	size += ft_strlen(str2);
	if (!(str = (char*)malloc(sizeof(char) * (size + 1))))
		return (NULL);
	ft_bzero(str, size + 1);
	ft_strcat(str, str1);
	ft_strcat(str, "=");
	ft_strcat(str, str2);
	str[size] = '\0';
	return (str);
}

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
			ft_dprintf(2, "%s: warning: shell level (%zu) too high, resetting to 1\n", env->prog_name, value);
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

int				ft_envp_mandatory(t_env *env)
{
	if (!(ft_envp_shlevel(env)))
		return (0);
	if (!(ft_envp_getpwd(env)))
		return (0);
	if (!(ft_envp_get_(env)))
		return (0);
	return (1);
}

int				ft_envp_copy(t_env *env, char **envp, int size)
{
	int			i;

	if (!(env->envp.env = (char**)malloc(sizeof(char*) * (size + 1))))
		return (ft_crash(MALLOC_FAIL, NULL, env));
	ft_bzero(env->envp.env, sizeof(char*) * (size + 1));
	i = 0;
	while (i < size)
	{
		if (!(env->envp.env[i] = ft_strdup(envp[i])))
			return (ft_crash(MALLOC_FAIL, NULL, env));
		i++;
	}
	env->envp.size = size;
	env->envp.occupied = i;
	return (1);
}

int				ft_envp_alloc_and_copy(t_env *env, char **envp)
{
	int			i;

	if (envp == NULL || envp[0] == NULL)
		return (ft_envp_realloc(env, 10));
	i = 0;
	while (envp[i])
		i++;
	return (ft_envp_copy(env, envp, i));
}

int				ft_env_init(t_env *env, char **av, char **envp)
{
	env->prog_name = av[0];
	if (!(ft_envp_alloc_and_copy(env, envp)))
		return (0);
	if (!(ft_envp_mandatory(env)))
		return (0);
	return (1);
}
