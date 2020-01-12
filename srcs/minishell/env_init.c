/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toliver <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/12 01:48:16 by toliver           #+#    #+#             */
/*   Updated: 2020/01/12 04:32:01 by toliver          ###   ########.fr       */
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
}

void			ft_free_env(t_env *env)
{
	ft_free_envp(env);
}


// faire une fonction ou on passe 2 morceaux de phrase et elle ressort une phrase separee par un =

int				ft_envp_shlevel(t_env *env)
{
	// get le shlevel s'il est la et l'augmente de 1 si dispo, sinon le set a 1 si trop haut (limite 1316134913 a voir pokwa) ou si negatif initialise a 0
	(void)env;
	return (1);
}

int				ft_envp_getpwd(t_env *env)
{
	// juste une fois au demarrage
	(void)env;
	return (1);
}

int				ft_envp_get_(t_env *env)
{
	// a chaque lancement de programme, prend le dernier param
	(void)env;
	return (1);
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
