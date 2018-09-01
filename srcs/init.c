/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toliver <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/01 02:22:43 by toliver           #+#    #+#             */
/*   Updated: 2018/09/02 01:29:14 by toliver          ###   ########.fr       */
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

char			*capitalize(char *str)
{
	int			i;

	i = 0;
	while (str[i])
	{
		if (str[i] >= 'a' && str[i] <= 'z')
			str[i] -= 32;
		i++;
	}
	return (str);
}

int				addenvequal(char *variable, t_envs *env)
{
	char		*name;
	char		*value;
	int			i;

	i = 0;
	while (variable[i] != '=')
		i++;
	if (!(name = (char*)malloc(sizeof(char) * (i + 1))))
	{
		env->running = -1;
		return (-1);
	}
	ft_strncpy(name, variable, i);
	name[i] = '\0';
	capitalize(name);
	if (!(value = ft_strdup(variable + i + 1)))
	{
		env->running = -1;
		return (-1);
	}
	if (addenvvar(name, variable + i + 1, env) == -1)
		return (-1);
	free(name);
	return (1);
}

int				addenv(t_var **list, t_var *node)
{
	t_var		*tmp;

	tmp = *list;
	while (tmp && tmp->next && tmp->name != node->name)
		tmp = tmp->next;
	if (!tmp)
		*list = node;
	else if (!ft_strcmp(tmp->name, node->name))
	{
		free(node->name);
		free(tmp->value);
		tmp->value = node->value;
		free(node);
	}
	else
		tmp->next = node;
	return (1);
}

int				addenvvar(char *name, char *value, t_envs *env)
{
	t_var		*var;

	if (!(var = (t_var*)malloc(sizeof(t_var))))
	{
		env->running = -1;
		return (-1);
	}
	if (!(var->name = ft_strdup(name)))
	{
		env->running = -1;
		return (-1);
	}
	if (value)
	{
		if (!(var->value = ft_strdup(value)))
		{
			env->running = -1;
			return (-1);
		}
	}
	else
	{	
		if (!(var->value = (char*)malloc(sizeof(char))))
		{
			env->running = -1;
			return (-1);
		}
		var->value[0] = '\0';
	}
	var->next = NULL;
	addenv(&env->envp, var);
	return (1);
}

int				copyenv(char **envp, t_envs *env)
{
	int			i;

	i = 0;
	while (envp && envp[i])
	{
		if (addenvequal(envp[i], env) == -1)
			return (0);
		i++;
	}
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
