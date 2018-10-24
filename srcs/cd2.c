/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd2.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toliver <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/28 17:52:14 by toliver           #+#    #+#             */
/*   Updated: 2018/10/24 13:35:03 by toliver          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int				swapoldpwd(t_envs *env)
{
	char		*oldpwd;
	char		*pwd;

	oldpwd = NULL;
	pwd = NULL;
	if (!pwdexist(env->envp))
	{
		if (!(oldpwd = getcwd(NULL, 0)))
			returnval(-1, env);
	}
	else
		oldpwd = ft_strdup(getvarvalue("PWD", env->envp));
	if (envvarexist("OLDPWD", env->envp))
		if (!(pwd = ft_strdup(getvarvalue("OLDPWD", env->envp))))
			returnval(-1, env);
	addenvvar("OLDPWD", oldpwd, env);
	addenvvar("PWD", pwd, env);
	free(oldpwd);
	free(pwd);
	return (1);
}

int				cdabsolute(char *path, t_envs *env)
{
	if (path && path[ft_strlen(path) - 1] == '/' && ft_strlen(path) != 1)
		path[ft_strlen(path) - 1] = '\0';
	if (isvalidpath(path))
	{
		if (swapoldpwd(env) == -1 || addenvvar("PWD", path, env) == -1)
			returnval(-1, env);
	}
	return (1);
}

int				gobackpath(char *path)
{
	int			i;

	i = ft_strlen(path);
	if (i > 0)
		i--;
	while (path[i] != '/' && i > 0)
		i--;
	if (i > 0)
		path[i] = '\0';
	else
		path[1] = '\0';
	return (1);
}

int				cdend(char *pathtofree, char *realpath, t_envs *env)
{
	free(pathtofree);
	if (isvalidpath(realpath))
	{
		if (swapoldpwd(env) == -1 || addenvvar("PWD", realpath, env) == -1)
			returnval(-1, env);
		free(realpath);
	}
	else
		free(realpath);
	return (1);
}
