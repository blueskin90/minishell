/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toliver <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/17 10:36:00 by toliver           #+#    #+#             */
/*   Updated: 2020/02/04 18:19:49 by toliver          ###   ########.fr       */
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

char				*ft_envp_get_value(char **envp, char *value)
{
	int				i;
	int				valuelen;

	valuelen = ft_strlen(value);
	i = 0;
	while (envp && envp[i])
	{
		if (ft_strnequ(envp[i], value, valuelen)
				&& envp[i][valuelen] == '=')
			return (&envp[i][0] + valuelen + 1);
		i++;
	}
	return (NULL);
}
