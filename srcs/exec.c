/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toliver <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/08 16:31:40 by toliver           #+#    #+#             */
/*   Updated: 2018/09/12 21:23:17 by toliver          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			freeenv(char **env)
{
	int		i;

	i = 0;
	while (env[i])
	{
		free(env[i]);
		env[i] = NULL;
		i++;
	}
	free(env);
	env = NULL;
	return (1);
}

int			getenvsize(t_envs *env)
{
	t_var	*ptr;
	int		i;

	ptr = env->envp;
	i = 0;
	while (ptr)
	{
		i++;
		ptr = ptr->next;
	}
	return (i);
}

int			copyname(char *dest, t_var *ptr)
{
	ft_strcpy(dest, ptr->name);
	ft_strcat(dest, "=");
	ft_strcat(dest, ptr->value);
	return (1);
}

char		**mallocenv(t_envs *env)
{
	char	**newenv;
	int		i;
	t_var	*ptr;

	if (!(newenv = (char**)malloc(sizeof(char*) * (getenvsize(env) + 1))))
	{
		env->running = MALLOC_FAILED;
		return (NULL);
	}
	i = 0;
	ptr = env->envp;
	while (ptr)
	{
		if (!(newenv[i] = (char*)malloc(sizeof(char)
						* (ft_strlen(ptr->name) + ft_strlen(ptr->value) + 2))))
		{
			env->running = MALLOC_FAILED;
			return (NULL);
		}
		copyname(newenv[i], ptr);
		i++;
		ptr = ptr->next;
	}
	newenv[i] = NULL;
	return (newenv);
}

int			exec(char *path, char **argv, t_envs *env)
{
	int		test;
	pid_t	forked;
	char	**newenv;

	if ((forked = fork()) == -1)
		returnval(-4, env);
	newenv = mallocenv(env);
	if (env->running < 0)
		return (-1);
	if (forked == 0 && env->running > 0)
	{
		execve(path, argv, newenv);
		exit(1);
	}
	else
		wait(&test);
	freeenv(newenv);
	return (1);
}

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
