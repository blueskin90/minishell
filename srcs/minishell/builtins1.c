/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toliver <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/17 09:30:50 by toliver           #+#    #+#             */
/*   Updated: 2020/01/17 11:38:03 by toliver          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int				ft_exec_internal(char *path, char **argv, char **env_copy,
		t_env *env)
{
	int			test;

	if (!(ft_check_path(path, env)))
		return (1);
	if ((env->pid = fork()) == -1)
	{
		ft_warning(EXEC, FORK_FAILED, path, env);
		env->pid = 0;
		return (1);
	}
	if (env->pid == 0)
	{
		execve(path, argv, env_copy);
		exit(1);
	}
	else
		wait(&test);
	env->pid = 0;
	return (1);
}

int				ft_free_paths(char **paths)
{
	int			i;

	i = 0;
	while (paths && paths[i])
	{
		free(paths[i]);
		i++;
	}
	free(paths);
	return (1);
}

int				ft_exec_findpath_fuck_the_norm(t_env *env, char *execpath)
{
	if (access(execpath, F_OK) == 0)
	{
		free(env->command[0]);
		env->command[0] = execpath;
		return (0);
	}
	return (1);
}

int				ft_exec_findpath(t_env *env)
{
	char		*path;
	char		**paths;
	char		*execpath;
	int			i;

	execpath = NULL;
	path = ft_env_get_value(env, "PATH");
	if (path == NULL)
		return (0);
	if (!(paths = ft_split_charset(path, ":")))
		return (-1);
	i = 0;
	while (paths && paths[i])
	{
		if (!(execpath = ft_path_combine(paths[i], env->command[0], "/")))
		{
			ft_free_paths(paths);
			return (-1);
		}
		if (!ft_exec_findpath_fuck_the_norm(env, execpath))
			break ;
		free(execpath);
		i++;
	}
	return (ft_free_paths(paths));
}

int				ft_exec(t_env *env)
{
	char		**envcpy;
	int			retval;

	if (!(envcpy = ft_env_cpy(env)))
		return (ft_crash(MALLOC_FAIL, NULL, env));
	if (ft_is_path(env->command[0]))
		ft_exec_internal(env->command[0], env->command, envcpy, env);
	else
	{
		retval = ft_exec_findpath(env);
		if (retval == -1 || retval == 0)
		{
			ft_env_cpy_free(envcpy);
			return (retval == -1 ? ft_crash(MALLOC_FAIL, NULL, env) :
					ft_check_path_error(env->command[0], env, NOT_FOUND));
		}
		ft_exec_internal(env->command[0], env->command, envcpy, env);
	}
	ft_env_cpy_free(envcpy);
	return (1);
}
