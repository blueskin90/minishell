/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toliver <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/01 01:19:14 by toliver           #+#    #+#             */
/*   Updated: 2018/09/01 03:52:10 by toliver          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libftprintf.h"
# include "libft.h"

# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/uio.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <signal.h>
# include <stdlib.h>
# include <unistd.h>
# include <dirent.h>

# define MALLOC_FAILED -1
# define ENV_LOADFAIL -2

typedef struct	s_var
{
	char		*name;
	char		*value;
}				t_var;

typedef	struct	s_envs
{
	int			running;
	char		**envp;
}				t_envs;

/*
**	Exiting functions
*/

int				exitshell(t_envs *env);

/*
**	Loop functions
*/

int				loop(t_envs *env);

/*
**	Input functions
*/

int				execline(t_envs *env, char *line);

/*
**	Executable functions
*/

int				printenv(t_envs *env);

/*
**	Initialiation functions
*/

int				init(int argc, char **argv, char **envp, t_envs *env);
int				copyenv(char **envp, t_envs *env);
int				setoptions(int argc, char **argv, t_envs *env);

#endif
