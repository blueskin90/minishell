/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toliver <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/07 00:29:48 by toliver           #+#    #+#             */
/*   Updated: 2020/01/17 07:46:47 by toliver          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int				ft_echo_internal(t_env *env)
{
	int			i;

	i = 1;
	while (env->command[i])
	{
		ft_printf("%s%c", env->command[i], env->command[i + 1] ? ' ' : '\0');
		i++;
	}
	ft_printf("\n");
	return (1);
}

int				ft_env_internal(t_env *env)
{
	ft_env(env);
	return (1);
}

int				ft_setenv_internal(t_env *env)
{
	int			i;
	char		*command;

	i = 1;
	while (env->command[i])
	{
		if (!(command = ft_strdup(env->command[i])))
			return (ft_crash(MALLOC_FAIL, NULL, env));
		if (!(ft_setenv(env, command)))
			return (0);	
		i++;
	}
	return (1);
}

int				ft_unsetenv_internal(t_env *env)
{
	int			i;
	char		*command;

	i = 1;
	while (env->command[i])
	{
		if (!(command = ft_strdup(env->command[i])))
			return (ft_crash(MALLOC_FAIL, NULL, env));
		if (!(ft_unsetenv(env, command)))
		{
			free(command);
			return (0);
		}
		i++;
		free(command);
	}
	return (1);
}

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

int				ft_cd_internal(t_env *env)
{
	if (env->command[1] == NULL)
		return (ft_cd_home(env));
	else if (env->command[1] && env->command[1][0] == '-'
			&& env->command[1][1] == '\0')
		return (ft_cd_minus_internal(env));
	else
		return (ft_cd(env, env->command[1]));
	return (1);
}

int				ft_exec_internal(char *path, char **argv, char **env_copy, t_env *env)
{
	pid_t		forked;
	int			test;
	
	if (!(ft_check_path(path, env)))
		return (1);
	if ((forked = fork()) == -1)
		return (ft_warning(EXEC, FORK_FAILED, path, env)); // FORK QUI CRASH A GERER
	if (forked == 0)
	{
		execve(path, argv, env_copy);
		exit(1); // voir si il ne faut pas free
	}
	else
		wait(&test); // gerer le ctrl c qui kill le fils
	// je gere pas grand chose par ici :/ a revoir quand tout marchera
	return (1);
}

int				ft_is_path(char *str)
{
	if (str && (str[0] == '/' || str[0] == '.'))
		return (1);
	return (0);
}

void			ft_free_paths(char **paths)
{
	int			i;

	i = 0;
	while (paths && paths[i])
	{
		free(paths[i]);
		i++;
	}
	free(paths);
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
		if (access(execpath, F_OK) == 0)
		{
			free(env->command[0]);
			env->command[0] = execpath;
			break;
		}
		free(execpath);
		i++;
	}
	ft_free_paths(paths);
	return (1);
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
	return (1); // verifier que bien tout soit free
}

int				ft_exec_command(t_env *env)
{
	char		*str;

	str = env->command[0];
	if (str == NULL)
		return (1);
	if (!str[0])
		return (1);
	if (!(ft_envp_set_(env)))
		return (0);
	if (ft_strnequ("env", str, 3) && !str[3])
		return (ft_env_internal(env));
	if (ft_strnequ("unsetenv", str, 8) && !str[8])
		return (ft_unsetenv_internal(env));
	if (ft_strnequ("setenv", str, 6) && !str[6])
		return (ft_setenv_internal(env));
	if (ft_strnequ("echo", str, 4) && !str[4])
		return (ft_echo_internal(env));
	if (ft_strnequ("cd", str, 2) && !str[2])
		return (ft_cd_internal(env));
	if (ft_strnequ("exit", str, 4) && !str[4])
	{
		ft_printf("exit\n");
		return (0);
	}
	return (ft_exec(env));

	// rajouter dans le _ le dernier param (probablement pas oblige)!
}

int				ft_split_command(t_env *env, char *str)
{
	ft_free_command(env->command);
	if (!(env->command = ft_split_charset(str, "\t\n\r\v\f ")))
		return (ft_crash(MALLOC_FAIL, NULL, env));
	return (1);
}

char			*ft_expand_tilde(t_env *env, char *str)
{
	char		*home;
	int			i;
	int			count;
	char		*expanded;

	home = ft_env_get_value(env, "HOME");
	i = 0;
	count = 0;
	while (str[i])
	{
		if (str[i] == '~')
			count++;
		i++;
	}
	if (count && home == NULL)
	{
		ft_warning(EXPAND, MISSING_HOME, NULL, env); 
	}
	if (home == NULL)
		home = "";
	if (!(expanded = (char*)malloc(sizeof(char) * (count * ft_strlen(home) + i + 1))))
		return (NULL);
	ft_bzero(expanded, count * ft_strlen(home) + i + 1);
	i = 0;
	while (*str)
	{
		if (*str == '~')
		{
			ft_strcat(expanded, home);
			i += ft_strlen(home);
		}
		else
		{
			expanded[i] = *str;
			i++;
		}
		str++;
	}
	expanded[i] = '\0';
	return (expanded);
}

char			**ft_split_variables_free(char **tab)
{
	int			i;

	i = 0;
	while (tab && tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
	return (NULL);
}

char			**ft_split_variables_alloc(char *str, int count)
{
	char		**tab;
	int			num;
	int			i;

	i = 0;
	if (!(tab = (char**)malloc(sizeof(char*) * (count + 1))))
		return (NULL);
	ft_bzero(tab, sizeof(char*) * (count + 1));
	num = 0;
	while (str && str[i])
	{
		while (str[i] && str[i] != '$')
			i++;
		if (i != 0)
		{
			if (!(tab[num] = (char*)malloc(sizeof(char) * (i + 1))))
				return (ft_split_variables_free(tab));
			ft_strncpy(tab[num], str, i);
			tab[num][i] = '\0';
			num++;
			str += i;
			i = 0;
		}
		if (str[i] == '$')
		{
			i++;
			while (str[i] && !ft_is_charset(str[i], "\t\n\r\v\f $"))
				i++;
			if (!(tab[num] = (char*)malloc(sizeof(char) * (i + 1))))
				return (ft_split_variables_free(tab));
			ft_strncpy(tab[num], str, i);
			tab[num][i] = '\0';
			num++;
			str += i;
			i = 0;
		}
	}
	return (tab);
}

char			**ft_split_variables(char *str)
{
	int			i;
	int			count;

	i = 0;
	count = 1;
	while (str && str[i])
	{
		if (str[i] == '$')
		{
			count++;
			i++;
			while (str[i] && !ft_is_charset(str[i], "\t\n\r\v\f $"))
				i++;
			if (str[i])
				count++;
		}
		else
			i++;
	}
	return (ft_split_variables_alloc(str, count));
}

int				ft_expand_replace_variable(t_env *env, char **tab)
{
	int			i;
	char		*value;

	i = 0;
	while (tab && tab[i])
	{
		if (tab[i][0] == '$')
		{
			value = ft_env_get_value(env, tab[i] + 1);
			if (value == NULL && !(value = ft_strdup("")))
				return (0);
			else
			{
				if (!(value = ft_strdup(value)))
					return (0);
			}
			free(tab[i]);
			tab[i] = value;
		}
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

void			ft_run(t_env *env)
{
	int			retval;
	char		*str;

	ft_printf("$>");
	while ((retval = ft_gnl(0, &str)) > -1)
	{
		if (str == NULL && !(str = ft_strdup("")))
		{
			retval = -1;
			break;
		}
		if (!(ft_split_commands(env, str)))
			break;
		if (!(ft_exec_commands(env)))
			break;
		if (retval == 0)
			ft_printf("\n");
		ft_printf("$>");
	}
	if (retval == -1)
		ft_crash(MALLOC_FAIL, NULL, env);
}

int				main(int ac, char **av, char **envp)
{
	t_env		env;

	if (ac != 1)
	{
		ft_printf("%d\n", ac);
		return (ft_usage());
	}
	if (!(ft_env_init(&env, av, envp)))
		return (-1);
	ft_run(&env);
	ft_free_env(&env);
	return (0);
}
