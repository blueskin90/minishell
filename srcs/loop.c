/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toliver <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/01 02:23:08 by toliver           #+#    #+#             */
/*   Updated: 2018/09/05 01:59:03 by toliver          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
int				freesplittedline(char ***splittedline)
{
	int			i;

	i = 0;
	while ((*splittedline)[i])
	{
		free((*splittedline)[i]);
		i++;
	}
	free(*splittedline);
	return (1);
}
int				execline(t_envs *env, char *line)
{
	char		**splittedline;

	if (splitinstructions(line, &splittedline) == -1)
		env->running = -1;
	if (!splittedline || !splittedline[0])
		return (1);
	if (ft_strcmp(splittedline[0], "env") == 0)
		envshell(splittedline, env);
	else if (ft_strcmp(splittedline[0], "echo") == 0)
		echo(splittedline, env);
	else if (ft_strcmp(splittedline[0], "setenv") == 0)
		setenvshell(splittedline + 1, env);
	else if (ft_strcmp(splittedline[0], "unsetenv") == 0)
		unsetenvshell(splittedline + 1, env);
	else if (ft_strcmp(splittedline[0], "pwd") == 0)
		pwdshell(splittedline + 1, env);
	else if (ft_strcmp(splittedline[0], "exit") == 0)
		exitshell(env);
	else if (ft_strcmp(splittedline[0], "cd"))
		cdshell(splittedline + 1, env);
	else // tenter l'execution sinon command not found
		ft_printf("command not found: %s\n", splittedline[0]);
	freesplittedline(&splittedline);
	return (1);
}

int				loop(t_envs *env)
{
	char		*str;

	while (env->running > 0)
	{
		ft_putstr("$>");
		if (ft_get_next_line(1, &str) == -1)
			env->running = MALLOC_FAILED;
		execline(env, str);
		free(str);
	}
	return (1);
}
