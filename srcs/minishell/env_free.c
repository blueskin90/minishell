/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_free.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toliver <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/12 06:49:13 by toliver           #+#    #+#             */
/*   Updated: 2020/01/12 06:49:31 by toliver          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void			ft_free_command(t_env *env)
{
	free(env->command);
	env->command = NULL;
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
	ft_free_command(env);
}
