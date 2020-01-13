/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toliver <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/07 00:29:48 by toliver           #+#    #+#             */
/*   Updated: 2020/01/13 09:15:01 by toliver          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int				ft_echo_internal(t_env *env)
{
	int			i;

	i = 1;
	while (env->command[i])
	{
		ft_printf("%s%c", env->command[i], env->command[i + 1] ? ' ' : '\0');
		i++;
	}
	ft_printf("\n");
	return (1);
}

int				ft_env_internal(t_env *env)
{
	ft_env(env);
	return (1);
}

int				ft_setenv_internal(t_env *env)
{
	int			i;
	char		*command;

	i = 1;
	while (env->command[i])
	{
		if (!(command = ft_strdup(env->command[i])))
			return (ft_crash(MALLOC_FAIL, NULL, env));
		if (!(ft_setenv(env, command)))
			return (0);	
		i++;
	}
	return (1);
}

int				ft_unsetenv_internal(t_env *env)
{
	int			i;
	char		*command;

	i = 1;
	while (env->command[i])
	{
		if (!(command = ft_strdup(env->command[i])))
			return (ft_crash(MALLOC_FAIL, NULL, env));
		if (!(ft_unsetenv(env, command)))
		{
			free(command);
			return (0);
		}
		i++;
		free(command);
	}
	return (1);
}

void			ft_dump_command(t_env *env)
{
	int			i;

	i = 0;
	while (env->command && env->command[i])
	{
		ft_printf(">%s<\n", env->command[i]);
		i++;
	}
}

int				ft_cd_internal(t_env *env)
{
	(void)env;
	ft_printf("cd !\n");
	return (1);
}

int				ft_exec(t_env *env)
{
	(void)env;
	ft_printf("exec !\n");
//	ft_dump_command(env);
	return (1);
}

int				ft_exec_command(t_env *env)
{
	char		*str;

	str = env->command[0];
	if (str == NULL)
		return (1);
	if (!str[0])
		return (1);
	if (!(ft_envp_set_(env)))
		return (0);
	if (ft_strnequ("env", str, 3) && !str[3])
		return (ft_env_internal(env));
	if (ft_strnequ("unsetenv", str, 8) && !str[8])
		return (ft_unsetenv_internal(env));
	if (ft_strnequ("setenv", str, 6) && !str[6])
		return (ft_setenv_internal(env));
	if (ft_strnequ("echo", str, 4) && !str[4])
		return (ft_echo_internal(env));
	if (ft_strnequ("cd", str, 2) && !str[2])
		return (ft_cd_internal(env));
	if (ft_strnequ("exit", str, 4) && !str[4])
		return (0);
	return (ft_exec(env));

	// rajouter dans le _ le dernier param (probablement pas oblige)!
}

int				ft_split_command(t_env *env, char *str)
{
	ft_free_command(env->command);
	if (!(env->command = ft_split_charset(str, "\t\n\r\v\f ")))
		return (ft_crash(MALLOC_FAIL, NULL, env));
	return (1);
}

char			*ft_expand_tilde(t_env *env, char *str)
{
	char		*home;
	int			i;
	int			count;
	char		*expanded;

	home = ft_env_get_value(env, "HOME");
	i = 0;
	count = 0;
	while (str[i])
	{
		if (str[i] == '~')
			count++;
		i++;
	}
	if (count && home == NULL)
	{
		ft_warning(EXPAND, MISSING_HOME, NULL, env); 
	}
	if (home == NULL)
		home = "";
	if (!(expanded = (char*)malloc(sizeof(char) * (count * ft_strlen(home) + i + 1))))
		return (NULL);
	ft_bzero(expanded, count * ft_strlen(home) + i + 1);
	i = 0;
	while (*str)
	{
		if (*str == '~')
		{
			ft_strcat(expanded, home);
			i += ft_strlen(home);
		}
		else
		{
			expanded[i] = *str;
			i++;
		}
		str++;
	}
	expanded[i] = '\0';
	return (expanded);
}

char			**ft_split_variables_free(char **tab)
{
	int			i;

	i = 0;
	while (tab && tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
	return (NULL);
}

char			**ft_split_variables_alloc(char *str, int count)
{
	char		**tab;
	int			num;
	int			i;

	i = 0;
	if (!(tab = (char**)malloc(sizeof(char*) * (count + 1))))
		return (NULL);
	ft_bzero(tab, sizeof(char*) * (count + 1));
	num = 0;
	while (str && str[i])
	{
		while (str[i] && str[i] != '$')
			i++;
		if (i != 0)
		{
			if (!(tab[num] = (char*)malloc(sizeof(char) * (i + 1))))
				return (ft_split_variables_free(tab));
			ft_strncpy(tab[num], str, i);
			tab[num][i] = '\0';
			num++;
			str += i;
			i = 0;
		}
		if (str[i] == '$')
		{
			i++;
			while (str[i] && !ft_is_charset(str[i], "\t\n\r\v\f $"))
				i++;
			if (!(tab[num] = (char*)malloc(sizeof(char) * (i + 1))))
				return (ft_split_variables_free(tab));
			ft_strncpy(tab[num], str, i);
			tab[num][i] = '\0';
			num++;
			str += i;
			i = 0;
		}
	}
	return (tab);
}

char			**ft_split_variables(char *str)
{
	int			i;
	int			count;

	i = 0;
	count = 1;
	while (str && str[i])
	{
		if (str[i] == '$')
		{
			count++;
			i++;
			while (str[i] && !ft_is_charset(str[i], "\t\n\r\v\f $"))
				i++;
			if (str[i])
				count++;
		}
		else
			i++;
	}
	return (ft_split_variables_alloc(str, count));
}

int				ft_expand_replace_variable(t_env *env, char **tab)
{
	int			i;
	char		*value;

	i = 0;
	while (tab && tab[i])
	{
		if (tab[i][0] == '$')
		{
			value = ft_env_get_value(env, tab[i] + 1);
			if (value == NULL && !(value = ft_strdup("")))
				return (0);
			else
			{
				if (!(value = ft_strdup(value)))
					return (0);
			}
			free(tab[i]);
			tab[i] = value;
		}
		i++;
	}
	return (1);
}

char			*ft_expand_fuse(char **splitted)
{
	int			i;
	int			size;
	char		*str;

	size = 0;
	i = 0;
	while (splitted && splitted[i])
	{
		size += ft_strlen(splitted[i]);
		i++;
	}
	if (!(str = (char*)malloc(sizeof(char) * (size + 1))))
	{
		ft_split_variables_free(splitted);
		return (NULL);
	}
	ft_bzero(str, size + 1);
	i = 0;
	while (splitted && splitted[i])
	{
		ft_strcat(str, splitted[i]);
		i++;
	}
	ft_split_variables_free(splitted);
	return (str);
}

char			*ft_expand_variable(t_env *env, char *str)
{
	char		**splitted;

	if (!(splitted = ft_split_variables(str)))
	{
		ft_crash(MALLOC_FAIL, NULL, env);
		return (NULL);
	}
	if (!(ft_expand_replace_variable(env, splitted)))
	{
		ft_split_variables_free(splitted);
		ft_crash(MALLOC_FAIL, NULL, env);
		return (NULL);
	}
	return (ft_expand_fuse(splitted));
}

char			*ft_expand_str(t_env *env, char *str)
{
	char		*expanded;

	if (!(str = ft_expand_tilde(env, str)))
		return (NULL);
	if (!(expanded = ft_expand_variable(env, str)))
	{
		free(str);
		return (NULL);
	}
	free(str);
	return (expanded);
}

int				ft_split_commands(t_env *env, char *str)
{
	char		*expanded;

	ft_free_command(env->commands);
	if (!(expanded = ft_expand_str(env, str)))
	{
		free(str);
		return (ft_crash(MALLOC_FAIL, NULL, env));
	}
	free(str);
	if (!(env->commands = ft_split_charset(expanded, ";")))
	{
		free(expanded);
		return (ft_crash(MALLOC_FAIL, NULL, env));
	}
	free(expanded);
	return (1);
}

int				ft_exec_commands(t_env *env)
{
	int			i;

	i = 0;
	while (env->commands && env->commands[i])
	{
		if (!(ft_split_command(env, env->commands[i])))
			return (ft_crash(MALLOC_FAIL, NULL, env));
		if (!(ft_exec_command(env)))
			return (0);
		i++;
	}
	return (1);
}

void			ft_run(t_env *env)
{
	int			retval;
	char		*str;

	ft_printf("$>");
	while ((retval = ft_gnl(0, &str)) > -1)
	{
		if (str == NULL && !(str = ft_strdup("")))
		{
			retval = -1;
			break;
		}
		if (!(ft_split_commands(env, str)))
			break;
		if (!(ft_exec_commands(env)))
			break;
		if (retval == 0)
			ft_printf("\n");
		ft_printf("$>");
	}
	if (retval == -1)
		ft_crash(MALLOC_FAIL, NULL, env);
}

int				main(int ac, char **av, char **envp)
{
	t_env		env;

	if (ac != 1)
		return (ft_usage());
	if (!(ft_env_init(&env, av, envp)))
		return (-1);
	ft_run(&env);
	ft_free_env(&env);
	return (0);
}
