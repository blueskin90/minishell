/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toliver <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/01 02:22:43 by toliver           #+#    #+#             */
/*   Updated: 2018/09/01 23:50:04 by toliver          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int				setoptions(int argc, char **argv, t_envs *env)
{
	(void)argc;
	(void)argv;
	(void)env;
	return (1);
}

int				addenvequal(char *variable, t_envs *env)
{
	(void)variable;
	(void)env;
	return (1);
}

int				addenvvar(char *name, char *value, t_envs *env)
{
	t_var		*var;
	t_var		*ptr;

	if (!(var = (t_envs*)malloc(sizeof(t_envs))))
		return (-1);
	var->name = name;
	if (value)
		var->value = value;
	else
	{	
		if (!(var->value = (char*)malloc(sizeof(char))))
			return (-1);
		var->value[0] = '\0';
	}
	var->next = NULL;
	ptr = env->envp;
	while (ptr->next)
		ptr = ptr->next;
	ptr->next = var;
	return (1);
}

int				copyenv(char **envp, t_envs *env)
{
	char		**copy;
	int			i;

	i = 0;
	while (envp && envp[i])
		i++;
	if (!(copy = (char**)malloc(sizeof(char*) * (i + 1))))
		return (0);
	i = 0;
	while (envp && envp[i])
	{
		if (!(copy[i] = ft_strdup(envp[i])))
			return (0);
		i++;
	}
	copy[i] = NULL;
	env->envp = copy;
	return (1);
}

int				init(int argc, char **argv, char **envp, t_envs *env)
{
	setoptions(argc, argv, env); // set les options au lancement s'il y en a
	if (!copyenv(envp, env)) // copie l'environnement dans le *env
	{
		env->running = -1;
		return (1);
	}
	if (!envp || envp[0] == NULL)
		env->running = -2;
	else
		env->running = 1;
	return (1);
}
