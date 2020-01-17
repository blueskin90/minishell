/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toliver <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/17 10:36:00 by toliver           #+#    #+#             */
/*   Updated: 2020/01/17 11:37:44 by toliver          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_env		**ft_get_envptr(void)
{
	static t_env	*env = NULL;

	return (&env);
}

t_env				*ft_get_env(void)
{
	return (*(ft_get_envptr()));
}

void				ft_set_env(t_env *env)
{
	*(ft_get_envptr()) = env;
}

int					ft_is_path(char *str)
{
	if (str && (str[0] == '/' || str[0] == '.'))
		return (1);
	return (0);
}
