/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toliver <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/01 02:23:47 by toliver           #+#    #+#             */
/*   Updated: 2018/09/08 16:55:08 by toliver          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		exitshell(t_envs *env)
{
	if (env->running == MALLOC_FAILED)
		ft_putstr_fd("Malloc Failed\n", 2);
	else if (env->running == ENV_LOADFAIL)
		ft_putstr_fd("Couldn't load environement\n", 2);
	else if (env->running == ENVPWD_LOADFAIL)
		ft_putstr_fd("Couldn't load environement PWD\n", 2);
	else if (env->running == FORK_FAIL)
		ft_putstr_fd("Couldn't fork to execute the process\n", 2);
	else
		exit(1);
	return (1);
}

/*
**	 penser a tout free
*/
