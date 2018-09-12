/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toliver <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/08 16:31:40 by toliver           #+#    #+#             */
/*   Updated: 2018/09/12 18:20:56 by toliver          ###   ########.fr       */
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

int			execbuiltin(char **splittedline, t_envs *env)
{
	int		test;
	char	**newenv;
	pid_t	forked;

	(void)env;
	(void)test;
	(void)newenv;
	(void)forked;
	if (envvarexist("PATH", env->envp))
	{
		ft_printf("go faire la fusion des path et des tests\n");
	}
	else
		ft_printf("minishell: command not found: %s\n", splittedline[0]);
	return (1);
}

int			execabsolute(char **splittedline, t_envs *env)
{
	int		test;
	pid_t	forked;
	char	**newenv;

	if (access(splittedline[0], F_OK) == -1)
		ft_printf("minishell: no such file or directory: %s\n", splittedline[0]);
	else if (access(splittedline[0], X_OK) == -1)
		ft_printf("minishell: permission denied: %s\n", splittedline[0]);
	else
	{
		if ((forked = fork()) == -1)
			returnval(-4, env);
		newenv = mallocenv(env);
		if (env->running < 0)
			return (-1);
		if (forked == 0 && env->running > 0)
		{
			execve(splittedline[0], splittedline, newenv);
			exit(1);
		}
		else
			wait(&test);
		freeenv(newenv);
	}
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
