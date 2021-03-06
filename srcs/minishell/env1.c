/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env1.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toliver <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/17 08:28:50 by toliver           #+#    #+#             */
/*   Updated: 2020/02/04 18:17:47 by toliver          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int				ft_addenv(t_env *env, char *value)
{
	if (env->envp.size == env->envp.occupied)
	{
		if (!(ft_envp_realloc(env, env->envp.size + 10)))
			return (ft_crash(MALLOC_FAIL, NULL, env));
	}
	env->envp.env[env->envp.occupied] = value;
	env->envp.occupied += 1;
	return (1);
}

void			ft_remove_env(t_env *env, int index)
{
	int			i;

	free(env->envp.env[index]);
	i = index;
	while (i < env->envp.occupied)
	{
		env->envp.env[i] = env->envp.env[i + 1];
		i++;
	}
	env->envp.occupied -= 1;
}

int				ft_unsetenv(t_env *env, char *value)
{
	int			index;

	if ((index = ft_env_get_index(env, value)) == -1)
		return (ft_warning(UNSETENV, MISSING_VARIABLE, value, env));
	ft_remove_env(env, index);
	return (1);
}

int				ft_setenv(t_env *env, char *param)
{
	char		*value;
	int			index;

	if ((value = ft_strchr(param, '=')) == NULL)
		return (ft_warning(SETENV, MISSING_EQUAL, param, env));
	value[0] = '\0';
	index = ft_env_get_index(env, param);
	value[0] = '=';
	if (index == -1)
		return (ft_addenv(env, param));
	free(env->envp.env[index]);
	env->envp.env[index] = param;
	return (1);
}

char			*ft_env_get_value(t_env *env, char *value)
{
	int			i;
	int			valuelen;

	valuelen = ft_strlen(value);
	i = 0;
	while (i < env->envp.occupied)
	{
		if (ft_strnequ(env->envp.env[i], value, valuelen)
				&& env->envp.env[i][valuelen] == '=')
			return (&env->envp.env[i][0] + valuelen + 1);
		i++;
	}
	return (NULL);
}
