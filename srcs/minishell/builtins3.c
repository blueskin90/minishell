/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toliver <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/17 09:36:51 by toliver           #+#    #+#             */
/*   Updated: 2020/01/17 09:38:23 by toliver          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int				ft_update_env(t_env *env, char *path, char *value)
{
	char		*fullvalue;

	if (!(fullvalue = ft_path_combine(value, path, "=")))
		return (0);
	if (!(ft_setenv(env, fullvalue)))
	{
		free(fullvalue);
		return (0);
	}
	return (1);
}

int				ft_update_pwd(t_env *env, char *oldpwd)
{
	char		*path;

	path = NULL;
	if (!(path = getcwd(NULL, 0)))
		return (0);
	if (oldpwd == NULL)
		oldpwd = "";
	if (ft_env_get_value(env, "PWD"))
	{
		if (!(ft_update_env(env, path, "PWD")))
			return (ft_crash(MALLOC_FAIL, NULL, env));
	}
	free(path);
	if (!(ft_update_env(env, oldpwd, "OLDPWD")))
		return (ft_crash(MALLOC_FAIL, NULL, env));
	return (1);
}

int				ft_cd(t_env *env, char *path)
{
	char		*oldpath;

	if (!ft_check_path(path, env))
		return (1);
	oldpath = getcwd(NULL, 0);
	if (chdir(path) == -1)
	{
		ft_printf("%s: couldn't cd to %s\n", env->prog_name, path);
		free(oldpath);
		return (1);
	}
	if (!(ft_update_pwd(env, oldpath)))
	{
		free(oldpath);
		ft_printf("ici 2 ffs\n");
		return (ft_crash(MALLOC_FAIL, NULL, env));
	}
	free(oldpath);
	return (1);
}

int				ft_cd_minus_internal(t_env *env)
{
	char		*prev;

	prev = ft_env_get_value(env, "OLDPWD");
	if (!prev || !prev[0])
		return (ft_warning(CD, MISSING_OLDPWD, NULL, env));
	return (ft_cd(env, prev));
}

int				ft_cd_home(t_env *env)
{
	char		*home;

	if (!(home = ft_env_get_value(env, "HOME")))
		return (ft_warning(CD, MISSING_HOME, NULL, env));
	return (ft_cd(env, home));
}
