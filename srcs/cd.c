/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toliver <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/05 01:58:44 by toliver           #+#    #+#             */
/*   Updated: 2018/09/12 20:03:18 by toliver          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int				isabsolute(char *str)
{
	if (str[0] == '/')
		return (1);
	return (0);
}

int				isvalidpath(char *path)
{
	if (access(path, F_OK) == -1)
		ft_printf("minishell: no such file or directory: %s\n", path);
	else if (access(path, X_OK) == -1)
		ft_printf("cd: permission denied: %s\n", path);
	else
		return (1);
	return (0);
}

int				swapoldpwd(t_envs *env)
{
	char		*oldpwd;
	char		*pwd;

	if (!(oldpwd = ft_strdup(getvarvalue("OLDPWD", env->envp))))
		returnval(-1, env);
	if (!(pwd = ft_strdup(getvarvalue("PWD", env->envp))))
		returnval(-1, env);
	addenvvar("OLDPWD", pwd, env);
	addenvvar("PWD", oldpwd, env);
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

int				cdwithpoint(char *path, t_envs *env, char *actualpath)
{
	int			i;
	int			pathstrlen;
	char		*newpath;

	i = 0;
	pathstrlen = ft_strlen(path);
	while (path[i] == '.' && path[i + 1] == '.'
			&& (path[i + 2] == '/' || path[i + 2] == '\0') && i < pathstrlen)
	{
		gobackpath(actualpath);
		i += (path[i + 2] == '\0') ? 2 : 3;
	}
	pathstrlen = ft_strlen(path + i);
	i = ft_strlen(actualpath + i) + ((ft_strlen(path) > 0) ? pathstrlen + 1 : 0);
	if (!(newpath = (char*)malloc(sizeof(char) * i)))
		return (returnval(-1, env));
	ft_strcpy(newpath, actualpath);
	if (pathstrlen)
	{
		ft_strcat(newpath, "/");
		ft_strcat(newpath, path);
	}
	free(actualpath);
	if (isvalidpath(newpath))
	{
		if (swapoldpwd(env) == -1 || addenvvar("PWD", newpath, env) == -1)
			returnval(-1, env);
		free(newpath);
	}
	else
		ft_printf("wrong path or you dont have rights to access: %s\n", path);
	return (1);
}

int				cdrelative(char *path, t_envs *env)
{
	char		*realpath;
	int			mallocsize;
	char		*actualpath;

	if (pwdexist(env->envp))
	{
		if (!(actualpath = ft_strdup(getvarvalue("PWD", env->envp))))
			returnval(-1, env);
		if (path[ft_strlen(path) - 1] == '/')
			path[ft_strlen(path) - 1] = '\0';
		if (path[0] == '.' && path[1] == '.'
				&& (path[2] == '/' || path[2] == '\0'))
			return (cdwithpoint(path, env, actualpath));
		mallocsize = ft_strlen(actualpath) + ft_strlen(path) + 2;
		if (!(realpath = (char*)malloc(sizeof(char) * mallocsize)))
			returnval(-1, env);
		ft_strcpy(realpath, actualpath);
		ft_strcat(realpath, "/");
		ft_strcat(realpath, path);
		free(actualpath);
		if (isvalidpath(realpath))
		{
			if (swapoldpwd(env) == -1 || addenvvar("PWD", realpath, env) == -1)
				returnval(-1, env);
			free(realpath);
		}
	}
	else
		ft_printf("cd with a relative path doesn't work if PWD is not set\n");
	return (1);
}

int				cdhome(char *linetoexpand, t_envs *env)
{
	char		*realpath;
	int			mallocsize;

	if (envvarexist("HOME", env->envp))
	{
		mallocsize = (linetoexpand ? ft_strlen(linetoexpand - 1) : 0);
		mallocsize += ft_strlen(getvarvalue("HOME", env->envp)) + 1;
		if (!(realpath = (char*)malloc(sizeof(char) * mallocsize)))
			returnval(-1, env);
		ft_strcpy(realpath, getvarvalue("HOME", env->envp));
		if (linetoexpand)
			ft_strcat(realpath, linetoexpand + 1);
		if (isvalidpath(realpath))
		{
			if (swapoldpwd(env) == -1 || addenvvar("PWD", realpath, env) == -1)
				returnval(-1, env);
			free(realpath);
		}
	}
	else
		ft_printf("Can't expand '~' if HOME is not set\n");
	return (1);
}

int				cdminus(t_envs *env)
{
	if (getvarvalue("OLDPWD", env->envp))
	{
		if (isvalidpath(getvarvalue("OLDPWD", env->envp)))
		{
			if (swapoldpwd(env) == -1)
				returnval(-1, env);
		}
		else
			ft_printf("OLDPWD is not a valid path\n");
	}
	else
		ft_printf("cd - is impossible without an OLDPWD\n");
	return (1);
}

int				cdshell(char **splittedline, t_envs *env)
{
	if (!splittedline[0] || splittedline[0][0] == '~')
		cdhome(splittedline[0], env);
	else if (splittedline[1])
		ft_printf("Usage: cd [dir]\n");
	else if (isabsolute(splittedline[0]))
		cdabsolute(splittedline[0], env);
	else if (splittedline[0][0] == '-' && splittedline[0][1] == '\0')
		cdminus(env);
	else
		cdrelative(splittedline[0], env);
	if (getvarvalue("PWD", env->envp))
		chdir(getvarvalue("PWD", env->envp));
	return (1);
}
