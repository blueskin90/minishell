/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setenv.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toliver <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/05 01:45:49 by toliver           #+#    #+#             */
/*   Updated: 2018/09/12 21:33:13 by toliver          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int				setenvshell(char **split, t_envs *env)
{
	int			i;

	i = 0;
	while (split[i])
		i++;
	if (i < 1 || i > 3)
		ft_printf(
		"Usage: setenv [varname] [varvalue] OR setenv [varname]=[varvalue]\n");
	else if (i == 1 && ft_strchr(split[0], '='))
		addenvequal(split[0], env);
	else if (i == 1 && !ft_strchr(split[0], '='))
		addenvvar(split[0], NULL, env);
	else if (i == 2 && ft_strchr(split[0], '='))
		ft_printf(
		"Usage: setenv [varname] [varvalue] OR setenv [varname]=[varvalue]\n");
	else
		addenvvar(capitalize(split[0]), split[1], env);
	return (1);
}
