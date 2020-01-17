/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins6.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toliver <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/17 09:44:44 by toliver           #+#    #+#             */
/*   Updated: 2020/01/17 10:34:52 by toliver          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char			*ft_expand_variable(t_env *env, char *str)
{
	char		**splitted;

	if (!(splitted = ft_split_variables(str)))
	{
		ft_crash(MALLOC_FAIL, NULL, env);
		return (NULL);
	}
	if (!(ft_expand_replace_variable(env, splitted)))
	{
		ft_split_variables_free(splitted);
		ft_crash(MALLOC_FAIL, NULL, env);
		return (NULL);
	}
	return (ft_expand_fuse(splitted));
}

char			*ft_expand_str(t_env *env, char *str)
{
	char		*expanded;

	if (!(str = ft_expand_tilde(env, str)))
		return (NULL);
	if (!(expanded = ft_expand_variable(env, str)))
	{
		free(str);
		return (NULL);
	}
	free(str);
	return (expanded);
}

int				ft_split_commands(t_env *env, char *str)
{
	char		*expanded;

	ft_free_command(env->commands);
	if (!(expanded = ft_expand_str(env, str)))
	{
		free(str);
		return (ft_crash(MALLOC_FAIL, NULL, env));
	}
	free(str);
	if (!(env->commands = ft_split_charset(expanded, ";")))
	{
		free(expanded);
		return (ft_crash(MALLOC_FAIL, NULL, env));
	}
	free(expanded);
	return (1);
}

int				ft_exec_commands(t_env *env)
{
	int			i;

	i = 0;
	while (env->commands && env->commands[i])
	{
		if (!(ft_split_command(env, env->commands[i])))
			return (ft_crash(MALLOC_FAIL, NULL, env));
		if (!(ft_exec_command(env)))
			return (0);
		i++;
	}
	return (1);
}

char			*ft_expand_fuse(char **splitted)
{
	int			i;
	int			size;
	char		*str;

	size = 0;
	i = 0;
	while (splitted && splitted[i])
	{
		size += ft_strlen(splitted[i]);
		i++;
	}
	if (!(str = (char*)malloc(sizeof(char) * (size + 1))))
	{
		ft_split_variables_free(splitted);
		return (NULL);
	}
	ft_bzero(str, size + 1);
	i = 0;
	while (splitted && splitted[i])
	{
		ft_strcat(str, splitted[i]);
		i++;
	}
	ft_split_variables_free(splitted);
	return (str);
}
