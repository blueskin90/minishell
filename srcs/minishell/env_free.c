/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_free.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toliver <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/12 06:49:13 by toliver           #+#    #+#             */
/*   Updated: 2020/01/12 17:04:53 by toliver          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void			ft_free_command(char **command)
{
	int			i;
	
	i = 0;
	while (command && command[i])
	{
		free(command[i]);
		i++;
	}
	free(command);
}

void			ft_free_envp(t_env *env)
{
	int			i;
	
	i = 0;
	while (i < env->envp.occupied && env->envp.env && env->envp.env[i])
	{
		free(env->envp.env[i]);
		i++;
	}
	free(env->envp.env);
	env->envp.env = NULL;
}

void			ft_free_env(t_env *env)
{
	ft_free_envp(env);
	ft_free_command(env->command);
	ft_free_command(env->commands);
}
