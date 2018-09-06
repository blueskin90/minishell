/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toliver <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/01 02:22:43 by toliver           #+#    #+#             */
/*   Updated: 2018/09/05 16:41:04 by toliver          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int				addenvequal(char *variable, t_envs *env)
{
	char		*name;
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
	if (addenvvar(name, variable + i + 1, env) == -1)
		return (-1);
	free(name);
	return (1);
}

int				addenv(t_var **list, t_var *node)
{
	t_var		*tmp;

	tmp = *list;
	while (tmp && tmp->next && ft_strcmp(tmp->name, node->name))
		tmp = tmp->next;
	if (!tmp)
		*list = node;
	else if (ft_strcmp(tmp->name, node->name) == 0)
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
		return (returnval(-1, env));
	if (!(var->name = ft_strdup(name)))
		return (returnval(-1, env));
	if (value)
	{
		if (!(var->value = ft_strdup(value)))
			return (returnval(-1, env));
	}
	else
	{
		if (!(var->value = (char*)malloc(sizeof(char))))
			return (returnval(-1, env));
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
	incrementshlevel(env);
	return (1);
}

int				init(int argc, char **argv, char **envp, t_envs *env)
{
	int			retval;

	setoptions(argc, argv, env);
	if ((retval = copyenv(envp, env)) == 0)
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
