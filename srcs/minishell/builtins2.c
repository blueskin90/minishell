/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toliver <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/17 09:33:58 by toliver           #+#    #+#             */
/*   Updated: 2020/01/17 09:35:43 by toliver          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void			ft_env_cpy_free(char **tofree)
{
	int			i;

	i = 0;
	while (tofree && tofree[i])
	{
		free(tofree[i]);
		i++;
	}
	free(tofree);
}

char			**ft_env_cpy(t_env *env)
{
	char		**envcpy;
	int			i;

	if (!(envcpy = (char**)malloc(sizeof(char*) * (env->envp.occupied + 1))))
		return (NULL);
	ft_bzero(envcpy, sizeof(char*) * (env->envp.occupied + 1));
	i = 0;
	while (i < env->envp.occupied)
	{
		if (!(envcpy[i] = ft_strdup(env->envp.env[i])))
		{
			ft_env_cpy_free(envcpy);
			return (NULL);
		}
		i++;
	}
	return (envcpy);
}

int				ft_check_path_error(char *path, t_env *env, int error)
{
	ft_dprintf(2, "%s: ", env->prog_name);
	if (error == DOESNT_EXIST)
		ft_dprintf(2, "no such file or directory: %s\n", path);
	else if (error == CANT_EXECUTE)
		ft_dprintf(2, "permission denied: %s\n", path);
	else if (error == NOT_FOUND)
		ft_dprintf(2, "command not found: %s\n", path);
	else
		ft_dprintf(2, "unknown error: %s\n", path);
	(void)env;
	return (1);
}

int				ft_check_path(char *path, t_env *env)
{
	int			ret;

	ret = access(path, F_OK);
	if (ret == -1)
	{
		ft_check_path_error(path, env, DOESNT_EXIST);
		return (0);
	}
	ret = access(path, X_OK);
	if (ret == -1)
	{
		ft_check_path_error(path, env, CANT_EXECUTE);
		return (0);
	}
	return (1);
}

char			*ft_path_combine(char *pathstart, char *pathend, char *pathlink)
{
	int			size;
	char		*fullpath;

	if (!pathstart || !pathend || !pathlink)
		return (0);
	size = ft_strlen(pathstart) + ft_strlen(pathend) + ft_strlen(pathlink);
	if (!(fullpath = (char*)malloc(sizeof(char) * (size + 1))))
		return (NULL);
	ft_bzero(fullpath, sizeof(char) * (size + 1));
	ft_strcat(fullpath, pathstart);
	ft_strcat(fullpath, pathlink);
	ft_strcat(fullpath, pathend);
	return (fullpath);
}
