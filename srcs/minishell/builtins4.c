/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins4.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toliver <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/17 09:38:46 by toliver           #+#    #+#             */
/*   Updated: 2020/01/25 17:03:40 by toliver          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int				ft_split_command(t_env *env, char *str)
{
	ft_free_command(env->command);
	if (!(env->command = ft_split_charset(str, "\t\n\r\v\f ")))
		return (ft_crash(MALLOC_FAIL, NULL, env));
	return (1);
}

char			*ft_fuse_array(char **arr, char *fuse)
{
	int			size;
	int			i;
	char		*str;

	i = 0;
	size = 0;
	while (arr[i])
	{
		size += ft_strlen(arr[i]);
		if (arr[i + 1])
			size += 1;
		i++;
	}
	size += 1;
	if (!(str = (char*)ft_memalloc(sizeof(char) * size)))
		return (NULL);
	i = 0;
	while (arr[i])
	{
		ft_strcat(str, arr[i]);
		if (arr[i + 1])
			ft_strcat(str, fuse);
		i++;
	}
	return (str);
}

char			*ft_expand_tilde_command_final(t_env *env, char *str)
{
	char		*finalstr;
	char		*home;
	int			size;

	if (!(str && str[0] == '~' && (!str[1] || str[1] == '/')))
		return (ft_strdup(str));
	if (!(home = ft_env_get_value(env, "HOME")))
		home = "";
	size = ft_strlen(str) + ft_strlen(home) + 1;
	if (!(finalstr = (char*)ft_memalloc(sizeof(char) * size)))
		return (NULL);
	ft_strcat(finalstr, home);
	ft_strcat(finalstr, str + 1);
	return (finalstr);
}

char			*ft_expand_tilde_command(t_env *env, char *str)
{
	char		**splitted_command;
	int			i;
	char		*tmp;

	if (!(splitted_command = ft_split_charset(str, " \t\n\r\v\f")))
		return (NULL);
	i = 0;
	while (splitted_command[i])
	{
		tmp = splitted_command[i];
		if (!(splitted_command[i] = ft_expand_tilde_command_final(env, tmp)))
		{
			splitted_command[i] = tmp;
			ft_split_free(splitted_command);
			return (NULL);
		}
		free(tmp);
		i++;
	}
	tmp = ft_fuse_array(splitted_command, " ");
	ft_split_free(splitted_command);
	return (tmp);
}

char			*ft_expand_tilde(t_env *env, char *str)
{
	char		**splitted_commands;
	int			i;
	char		*tmp;

	if (!(splitted_commands = ft_split_charset(str, ";")))
		return (NULL);
	i = 0;
	while (splitted_commands[i])
	{
		tmp = splitted_commands[i];
		if (!(splitted_commands[i] = ft_expand_tilde_command(env, tmp)))
		{
			splitted_commands[i] = tmp;
			ft_split_free(splitted_commands);
			return (NULL);
		}
		free(tmp);
		i++;
	}
	tmp = ft_fuse_array(splitted_commands, ";");
	ft_split_free(splitted_commands);
	return (tmp);
}
