/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toliver <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/17 08:36:53 by toliver           #+#    #+#             */
/*   Updated: 2020/01/17 08:39:38 by toliver          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int				ft_envp_realloc(t_env *env, int size)
{
	char		**tmp;
	int			i;

	if (!(tmp = (char**)malloc(sizeof(char*) * (size + 1))))
		return (ft_crash(MALLOC_FAIL, NULL, env));
	i = 0;
	ft_bzero(tmp, sizeof(char*) * (size + 1));
	while (i < env->envp.occupied)
	{
		tmp[i] = env->envp.env[i];
		i++;
	}
	env->envp.size = size;
	free(env->envp.env);
	env->envp.env = tmp;
	return (1);
}
