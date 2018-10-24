/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toliver <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/24 13:46:50 by toliver           #+#    #+#             */
/*   Updated: 2018/10/24 13:47:09 by toliver          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int				init_empty_env(t_envs *env)
{
	char		*path;

	path = getcwd(NULL, 0);
	if (path == NULL)
	{
		env->running = -2;
		return (1);
	}
	addenvvar("PWD", path, env);
	free(path);
	addenvvar("SHLVL", "1", env);
	addenvvar("_", "/usr/bin/env", env);
	return (1);
}
