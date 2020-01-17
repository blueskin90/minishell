/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toliver <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/17 08:29:39 by toliver           #+#    #+#             */
/*   Updated: 2020/01/17 13:19:14 by toliver          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int				ft_env_get_index(t_env *env, char *value)
{
	int			i;
	int			valuelen;

	valuelen = ft_strlen(value);
	i = 0;
	while (i < env->envp.occupied)
	{
		if (ft_strnequ(env->envp.env[i], value, valuelen)
				&& env->envp.env[i][valuelen] == '=')
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
		tmp[i] = ft_strdup(env[i]);
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
