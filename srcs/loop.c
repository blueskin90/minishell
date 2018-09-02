/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toliver <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/01 02:23:08 by toliver           #+#    #+#             */
/*   Updated: 2018/09/02 03:12:20 by toliver          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int				printenv(t_envs *env)
{
	t_var		*ptr;

	ptr = env->envp;
	while (ptr)
	{
		ft_printf("%s=%s\n", ptr->name, ptr->value);
		ptr = ptr->next;
	}
	return (1);
}

int				printvariable(char *str, int len, t_envs *env)
{
	char		c;
	t_var		*ptr;
	char		*name;

	c = str[len];
	str[len] = '\0';
	ptr = env->envp;
	name = ft_strdup(str);
	capitalize(name);
	while (ptr && ft_strcmp(name, ptr->name))
		ptr = ptr->next;
	if (ptr)
		ft_putstr(ptr->value);
	str[len] = c;
	return (1);
}

int				echowithvariable(char *str, t_envs *env)
{
	int			i;
	int			ii;

	i = 0;
	ii = 0;
	while (str[i] != '$')
	{
		ft_putchar(str[i]);
		i++;
	}
	i++;
	while (str[i + ii] && iswhitespace(str[i + ii]) == 0)
		ii++;
	printvariable(str + i, ii, env);
	if (str[i + ii])
		echostr(str + i + ii, env);
	return (1);
}

int				echostr(char *str, t_envs *env)
{
	int			i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && ((i > 0 && str[i] - 1 != '/') || i == 0))
		{
			echowithvariable(str, env);
			break;
		}
		if (str[i] == '\\')
			i++;
		ft_putchar(str[i]);
		i++;
	}
//	if (str[i] == '\0')
//		ft_putstr(str); // ne pas faire un putstr normal, escape les /
	return (1);
}

int				echo(char **instructions, t_envs *env)
{
	int			i;
	int			isfirst;

	i = 1;
	isfirst = 1;
	while (instructions && instructions[i])
	{
		if (isfirst == 0)
			ft_putchar(' ');
		else
			isfirst = 0;
		echostr(instructions[i], env);
		i++;
	}
	return (1);
}

int				isquote(char c)
{
	if (c == '\'' || c == '\"')
		return (1);
	return (0);
}

int				iswhitespace(char c)
{
	if (c == '\t' || c == '\n' || c == '\r' || c == '\v' || c == '\f'
			|| c == ' ')
		return (1);
	return (0); // attention, ptet pas return 1 si c'est /n
}

int				get_wordnumber(char *line)
{
	int			i;
	int			wrdnbr;
	char		lastquote;

	i = 0;
	wrdnbr = 0;
	while (line && line[i])
	{
		while (iswhitespace(line[i]))
			i++;
		if (line[i] && !iswhitespace(line[i]) && !isquote(line[i]) && ++wrdnbr)
		{
			while (line[i] && !iswhitespace(line[i]) && !isquote(line[i]))
				i++;
		}
		if (line[i] && isquote(line[i]) && ++wrdnbr)
		{
			lastquote = line[i];
			i++;
			while (line[i] && line[i] != lastquote)
				i++;
			if (line[i] == lastquote)
				i++;
		}
	}
	return (wrdnbr);
}

int				get_firstword(char *line)
{
	int			i;

	i = 0;
	while (line && line[i] && iswhitespace(line[i]))
		i++;
	return (i);
}

int				get_nextword(char *line)
{
	int			i;
	char		c;

	i = 0;
	if (isquote(*line))
	{
		c = *line;
		i++;
		while (line && line[i] && line[i] != c)
			i++;
		if (line && line[i] && line[i] == c)
			i++;
		while (line && line[i] && iswhitespace(line[i]))
			i++;
	}
	else
	{
		while (line && line[i] && !(iswhitespace(line[i])))
			i++;
		while (line && line[i] && iswhitespace(line[i]))
			i++;
	}
	return (i);
}

int				wordsize(char *line)
{
	int			i;
	char		c;

	i = 0;
	if (isquote(*line))
	{
		c = *line;
		i++;
		while (line && line[i] && line[i] != c)
			i++;
		if (line[i] == c)
			return (i - 1);
	}
	else
	{
		while (line && line[i] && !(iswhitespace(line[i]))
				&& !(isquote(line[i])))
			i++;
	}
	return (i);
}

int				wordcopy(char *line, char **dst)
{
	char		c;
	int			i;

	i = 0;
	if (isquote(*line))
	{
		c = *line;
		line++;
		while (line[i] && line[i] != c)
		{
			(*dst)[i] = line[i];
			i++;
		}
		(*dst)[i] = '\0';
		return (1);
	}
	while (line[i] && !iswhitespace(line[i]) && !isquote(line[i]))
	{
		(*dst)[i] = line[i];
		i++;
	}
	(*dst)[i] = '\0';
	return (1);
}

int				splitinstructions(char *line, char ***instructions)
{
	char		**split;
	int			wrdnumber;
	int			i;

	split = NULL;
	if ((wrdnumber = get_wordnumber(line)) != 0)
	{
		if (!(split = (char**)malloc(sizeof(char*) * (wrdnumber + 1))))
			return (-1);
		split[wrdnumber] = NULL;
		line += get_firstword(line);
		i = 0;
		while (*line)
		{
			if (!(split[i] = (char*)malloc(sizeof(char) * (wordsize(line) + 1))))
				return (-1);
			if (wordcopy(line, &split[i]) == -1)
				return (-1);
			line += get_nextword(line);
			i++;
		}
	}
	*instructions = split;
	return (1);
}

int				splitline(char *line, t_list *ptr)
{
	char		c;

	if (isquote(*line) && !(ptr->next = ft_lstnew(line + 1, wordsize(line) + 1)))
		return (-1);
	if (!isquote(*line) && !(ptr->next = ft_lstnew(line, wordsize(line) + 1)))
		return (-1);
	c = *line;
	if (isquote(*line))
		((char*)(ptr->next->content))[wordsize(line + 1)] = '\0';
	else
		((char*)(ptr->next->content))[wordsize(line)] = '\0';
	if (line[wordsize(line + 1)] != c)
		ft_printf("incomplete char = %c and end char = %c\n", c, line[wordsize(line + 1)]);
	return (1);
}

int				delnode(t_var *todel, t_var **list)
{
	t_var		*ptr;
	t_var		*tmp;

	if (*list == todel)
	{
		tmp = *list;
		*list = (*list)->next;
		free(tmp->name);
		free(tmp->value);
		free(tmp);
	}
	else
	{
		ptr = *list;
		while (ptr && ptr->next != todel)
			ptr = ptr->next;
		if (ptr->next == todel)
		{
			tmp = ptr->next;
			ptr->next = tmp->next;
			free(tmp->name);
			free(tmp->value);
			free(tmp);
		}
	}
	return (1);
}

int				unsetenvshell(char **split, t_envs *env)
{
	t_var		*ptr;
	int			i;

	i = 0;
	ptr = env->envp;
	if (ft_strcmp(split[0], "PWD") == 0)
		return (1);
	while (ptr && split[i])
	{
		capitalize(split[i]);
		if (ft_strcmp(split[i], "PWD") == 0)
			i++;
		else
		{
			if (ft_strcmp(ptr->name, split[i]) == 0)
			{
				delnode(ptr, &env->envp);
				ptr = env->envp;
			}
			ptr = ptr->next;
			if (ptr == NULL && split[i])
			{
				i++;
				ptr = env->envp;
			}
		}
	}	
	return (1);
}

int				setenvshell(char **split, t_envs *env)
{
	int			i;

	i = 0;
	while (split[i])
		i++;
	if (i < 1 || i > 3)
		ft_printf("Usage: setenv [varname] [varvalue] OR setenv [varname]=[varvalue]\n");
	else if (i == 1 && ft_strchr(split[0], '='))
		addenvequal(split[0], env);
	else if (i == 1 && !ft_strchr(split[0], '='))
		addenvvar(split[0], NULL, env);
	else if (i == 2 && ft_strchr(split[0], '='))
		ft_printf("Usage: setenv [varname] [varvalue] OR setenv [varname]=[varvalue]\n");
	else
		addenvvar(capitalize(split[0]), split[1], env);
	return (1);
}

int				pwdshell(char **splittedline, t_envs *env)
{
	t_var		*ptr;

	if (splittedline[0])
		ft_printf("pwd: too many arguments\n");
	else
	{
		ptr = env->envp;
		while (ptr && ft_strcmp(ptr->name, "PWD") != 0)
			ptr = ptr->next;
		if (ptr)
			ft_printf("%s\n", ptr->value);
		else
			ft_printf("env variable: $PWD is not set\n");
	}
	return (1);
}

int				freesplittedline(char ***splittedline)
{
	int			i;
	
	i = 0;
	while ((*splittedline)[i])
	{
		free((*splittedline)[i]);
		i++;
	}
	free(*splittedline);
	return (1);
}

int				execline(t_envs *env, char *line)
{
	char		**splittedline;

	if (splitinstructions(line, &splittedline) == -1)
		env->running = -1;
	if (!splittedline || !splittedline[0])
		return (1);
	if (ft_strcmp(splittedline[0], "env") == 0)
		printenv(env);
	else if (ft_strcmp(splittedline[0], "echo") == 0)
		echo(splittedline, env);
	else if (ft_strcmp(splittedline[0], "setenv") == 0)
		setenvshell(splittedline + 1, env);
	else if (ft_strcmp(splittedline[0], "unsetenv") == 0)
		unsetenvshell(splittedline + 1, env);
	else if (ft_strcmp(splittedline[0], "pwd") == 0)
		pwdshell(splittedline + 1, env);
	else if (ft_strcmp(splittedline[0], "exit") == 0)
		exitshell(env);
	else
		ft_printf("command not found: %s\n", splittedline[0]);
	freesplittedline(&splittedline);
	return (1);
}

int				loop(t_envs *env)
{
	char		*str;

	while (env->running > 0)
	{
		ft_putstr("$>");
		if (ft_get_next_line(1, &str) == -1)
			env->running = MALLOC_FAILED;
		execline(env, str);
		free(str);
	}
	return (1);
}
