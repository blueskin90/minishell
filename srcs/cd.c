/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toliver <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/05 01:58:44 by toliver           #+#    #+#             */
/*   Updated: 2018/10/23 21:45:20 by toliver          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	i = ft_strlen(actualpath + i) + ((ft_strlen(path) > 0)
			? pathstrlen + 1 : 0);
	if (!(newpath = (char*)malloc(sizeof(char) * i)))
		return (returnval(-1, env));
	ft_strcpy(newpath, actualpath);
	if (pathstrlen)
	{
		ft_strcat(newpath, "/");
		ft_strcat(newpath, path);
	}
	return (cdend(actualpath, newpath, env)); // peut etre free actualpath
}

int				cdrelative(char *path, t_envs *env)
{
	char		*realpath;
	int			mallocsize;
	char		*actualpath;

	if (!pwdexist(env->envp))
	{
		actualpath = getcwd(NULL, 0);
		if (!actualpath)
			returnval(-1, env);
	}
	else 
	{
		if (!(actualpath = ft_strdup(getvarvalue("PWD", env->envp))))
			returnval(-1, env);
	}
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
	if (cdend(actualpath, realpath, env) == -1)
		returnval(-1, env);
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
			ft_putendl(getvarvalue("PWD", env->envp));
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
