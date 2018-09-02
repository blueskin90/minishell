/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toliver <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/01 02:22:43 by toliver           #+#    #+#             */
/*   Updated: 2018/09/02 03:33:33 by toliver          ###   ########.fr       */
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

int				pwdexist(t_var *list)
{
	t_var		*ptr;

	ptr = list;
	while (ptr)
	{
		if (ft_strcmp(ptr->name, "PWD"))
			return(1);
		ptr = ptr->next;
	}
	return (0);
}

int				addenvequal(char *variable, t_envs *env)
{
	char		*name;
//	char		*value;
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
	if (addenvvar(name, variable + i + 1, env) == -1)
		return (-1);
	free(name);
	return (1);
}

int				addenv(t_var **list, t_var *node)
{
	t_var		*tmp;

	tmp = *list;
	while (tmp && tmp->next && ft_strcmp(tmp->name,node->name))
		tmp = tmp->next;
	if (!tmp)
		*list = node;
	else if (!ft_strcmp(tmp->name, node->name))
	{
		free(node->name);
		if (ft_strcmp(tmp->name, "PWD"))
		{
			free(tmp->value);
			tmp->value = node->value;
		}
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

int				incrementshlevel(t_envs *env)
{
	t_var		*ptr;
	char		*value;

	ptr = env->envp;
	while (ptr && ft_strcmp(ptr->name, "SHLVL"))
		ptr = ptr->next;
	if (ptr)
	{
		if (!(value = ft_itoa(ft_atoi(ptr->value) + 1)))
			env->running = -1;
		free(ptr->value);
		ptr->value = value;
	}
	else
		addenvvar("SHLVL", "1", env);
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
	if (!pwdexist(env->envp))
		return (-3);
	incrementshlevel(env);
	return (1);
}

int				init(int argc, char **argv, char **envp, t_envs *env)
{
	int			retval;
	setoptions(argc, argv, env); // set les options au lancement s'il y en a
	if ((retval = copyenv(envp, env)) == 0) // copie l'environnement dans le *env
	{
		env->running = -1;
		return (1);
	}
	else if (retval == -3)
	{
		env->running = -3;
		return (1);
	}
	if (!envp || envp[0] == NULL)
		env->running = -2;
	else
		env->running = 1;
	return (1);
}
