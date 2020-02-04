/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gnl2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toliver <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/31 15:38:45 by toliver           #+#    #+#             */
/*   Updated: 2020/01/31 15:42:16 by toliver          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int		ft_gnl_split(char **str, char **line)
{
	char		*split_pos;
	char		*tmp;

	if ((split_pos = ft_strchr(*str, '\n')) == NULL)
	{
		*line = *str;
		*str = NULL;
		return (*line ? 1 : 0);
	}
	split_pos[0] = '\0';
	if (!(*line = ft_strdup(*str)))
		return (-1);
	if (split_pos[1] == '\0')
	{
		free(*str);
		*str = NULL;
		return (1);
	}
	if (!(tmp = ft_strdup(split_pos + 1)))
		return (-1);
	free(*str);
	*str = tmp;
	return (1);
}

static int		ft_gnl_fill(char **str, int fd)
{
	char		buffer[1025];
	int			retval;
	char		*tmp;

	tmp = NULL;
	while ((retval = read(fd, buffer, 1024)) > 0)
	{
		buffer[retval] = '\0';
		if (!(tmp = ft_strjoin(*str, buffer)))
			return (-1);
		*str = tmp;
		tmp = NULL;
		if (ft_strchr(*str, '\n') != NULL)
			break ;
	}
	if (retval == -1)
		return (-1);
	return (retval);
}

int				ft_gnl2(int fd, char **line)
{
	static char	*str = NULL;
	int			retval;

	retval = 0;
	if (!line || fd < 0)
		return (-1);
	*line = NULL;
	if (ft_strchr(str, '\n') == NULL && ft_gnl_fill(&str, fd) == -1)
	{
		free(str);
		str = NULL;
		return (-1);
	}
	if ((retval = ft_gnl_split(&str, line)) == -1)
	{
		free(str);
		free(*line);
		str = NULL;
		*line = NULL;
		return (-1);
	}
	return (retval);
}
