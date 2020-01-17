/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toliver <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/12 01:50:21 by toliver           #+#    #+#             */
/*   Updated: 2020/01/17 07:46:45 by toliver          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
//add env et setenv ne malloc pas une copie de la value, passer directement une copie
// ou ne pas la free jsais pas encore

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

char*			ft_env_get_value(t_env *env, char *value)
{
	int			i;
	int			valuelen;

	valuelen = ft_strlen(value);
	i = 0;
	while (i < env->envp.occupied)
	{
		if (ft_strnequ(env->envp.env[i], value, valuelen) && env->envp.env[i][valuelen] == '=')
			return (&env->envp.env[i][0] + valuelen + 1);
		i++;
	}
	return (NULL);
}

int				ft_env_get_index(t_env *env, char *value)
{
	int			i;
	int			valuelen;

	valuelen = ft_strlen(value);
	i = 0;
	while (i < env->envp.occupied)
	{
		if (ft_strnequ(env->envp.env[i], value, valuelen) && env->envp.env[i][valuelen] == '=')
			return (i);
		i++;
	}
	return (-1);
}

void			ft_dump_env(char **env)
{
	int			i;

	i = 0;
	while (env && env[i])
	{
		ft_printf("%s\n", env[i]);
		i++;
	}
}

int				ft_env_getsize(char **env)
{
	int			i;

	i = 0;
	while (env && env[i])
		i++;
	return (i);
}

char			**ft_env_add_param(char **env, char *param)
{
	char		**tmp;
	int			i;

	if (!(tmp = (char**)malloc(sizeof(char*) * (ft_env_getsize(env) + 2))))
		return (NULL);
	i = 0;
	ft_bzero(tmp, sizeof(char*) * (ft_env_getsize(env) + 2));
	while (env && env[i])
	{
		tmp[i] = env[i];
		i++;
	}
	if (!(tmp[i] = ft_strdup(param)))
	{
		ft_env_cpy_free(tmp);
		return (NULL);
	}
	return (tmp);
}

int				ft_env_add_params(char ***envp, t_env *env, int *i)
{
	char		**tmp;

	while (ft_strchr(env->command[*i], '=') != NULL)
	{
		if (!(tmp = ft_env_add_param(*envp, env->command[*i])))
		{
			ft_env_cpy_free(*envp);
			*envp = NULL;
			return (0);
		}
		free(*envp);
		*envp = tmp;
		(*i)++;
	}
	return (1);
}

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

	tmp_env = NULL;
	i = 1;
	if (!env->command[1] || !ft_strequ(env->command[1], "-i"))
	{
		if (!(tmp_env = ft_env_cpy(env)))
			return (ft_crash(MALLOC_FAIL, NULL, env));
	}
	if (ft_strequ(env->command[1], "-i"))
	{
		i++;
		if (!(tmp_env = (char**)malloc(sizeof(char*))))
			return (ft_crash(MALLOC_FAIL, NULL, env));
		ft_bzero(tmp_env, sizeof(char*));
	}
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
