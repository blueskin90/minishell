/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toliver <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/08 16:31:40 by toliver           #+#    #+#             */
/*   Updated: 2018/09/28 17:49:16 by toliver          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			newpathalloc(char **newpath, char *path, char *endpath, t_envs *env)
{
	int		mallocsize;

	mallocsize = ft_strlen(path) + 2 + ft_strlen(endpath);
	if (!(*newpath = (char*)malloc(sizeof(char) * mallocsize)))
		returnval(-1, env);
	ft_strcpy(*newpath, path);
	ft_strcat(*newpath, "/");
	ft_strcat(*newpath, endpath);
	return (1);
}

int			execb(char *newpath, char **splittedline, t_envs *env, char **path)
{
	if (access(newpath, X_OK) == -1)
		ft_printf("minishell: permission denied: %s\n", splittedline[0]);
	else
		exec(newpath, splittedline, env);
	free(newpath);
	freeenv(path);
	return (1);
}

int			execbuiltin(char **splittedline, t_envs *env)
{
	char	**path;
	char	*newpath;
	int		i;

	path = NULL;
	newpath = NULL;
	i = 0;
	if (envvarexist("PATH", env->envp))
	{
		path = ft_strsplit(getvarvalue("PATH", env->envp), ':');
		while (path && path[i])
		{
			if (newpathalloc(&newpath, path[i], splittedline[0], env) == -1)
				returnval(-1, env);
			if (access(newpath, F_OK) != -1)
				return (execb(newpath, splittedline, env, path));
			free(newpath);
			i++;
		}
		ft_printf("minishell: command not found: %s\n", splittedline[0]);
		freeenv(path);
	}
	else
		ft_printf("minishell: command not found: %s\n", splittedline[0]);
	return (1);
}

int			execabsolute(char **splittedline, t_envs *env)
{
	if (access(splittedline[0], F_OK) == -1)
		ft_printf("minishell: no such file or directory: %s\n",
				splittedline[0]);
	else if (access(splittedline[0], X_OK) == -1)
		ft_printf("minishell: permission denied: %s\n", splittedline[0]);
	else
		exec(splittedline[0], splittedline, env);
	return (1);
}

int			execprogram(char **splittedline, t_envs *env)
{
	if (splittedline[0][0] == '/' || splittedline[0][0] == '.')
		execabsolute(splittedline, env);
	else
		execbuiltin(splittedline, env);
	return (1);
}
