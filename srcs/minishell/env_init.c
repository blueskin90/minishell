/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toliver <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/12 01:48:16 by toliver           #+#    #+#             */
/*   Updated: 2020/01/17 08:39:39 by toliver          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char			*ft_envp_concat(char *str1, char *str2)
{
	char		*str;
	int			size;

	size = 1;
	size += ft_strlen(str1);
	size += ft_strlen(str2);
	if (!(str = (char*)malloc(sizeof(char) * (size + 1))))
		return (NULL);
	ft_bzero(str, size + 1);
	ft_strcat(str, str1);
	ft_strcat(str, "=");
	ft_strcat(str, str2);
	str[size] = '\0';
	return (str);
}

int				ft_envp_mandatory(t_env *env)
{
	if (!(ft_envp_shlevel(env)))
		return (0);
	if (!(ft_envp_getpwd(env)))
		return (0);
	if (!(ft_envp_get_(env)))
		return (0);
	return (1);
}

int				ft_envp_copy(t_env *env, char **envp, int size)
{
	int			i;

	if (!(env->envp.env = (char**)malloc(sizeof(char*) * (size + 1))))
		return (ft_crash(MALLOC_FAIL, NULL, env));
	ft_bzero(env->envp.env, sizeof(char*) * (size + 1));
	i = 0;
	while (i < size)
	{
		if (!(env->envp.env[i] = ft_strdup(envp[i])))
			return (ft_crash(MALLOC_FAIL, NULL, env));
		i++;
	}
	env->envp.size = size;
	env->envp.occupied = i;
	return (1);
}

int				ft_envp_alloc_and_copy(t_env *env, char **envp)
{
	int			i;

	if (envp == NULL || envp[0] == NULL)
		return (ft_envp_realloc(env, 10));
	i = 0;
	while (envp[i])
		i++;
	return (ft_envp_copy(env, envp, i));
}

int				ft_env_init(t_env *env, char **av, char **envp)
{
	env->prog_name = av[0];
	if (!(ft_envp_alloc_and_copy(env, envp)))
		return (0);
	if (!(ft_envp_mandatory(env)))
		return (0);
	return (1);
}
