/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toliver <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/01 01:19:14 by toliver           #+#    #+#             */
/*   Updated: 2018/10/24 16:36:10 by toliver          ###   ########.fr       */
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
# define ENVPWD_LOADFAIL -3
# define FORK_FAIL -4

typedef struct	s_var
{
	char			*name;
	char			*value;
	struct s_var	*next;
}				t_var;

typedef	struct	s_envs
{
	int			running;
	t_var		*envp;
}				t_envs;

/*
**	Exiting functions
*/

int				exitshell(t_envs *env);

/*
**	Loop functions
*/

int				loop(t_envs *env);
int				splitline(char *line, t_list *ptr);
int				splitinstructions(char *line, char ***instruction, t_envs *env);
int				wordcopy(char *line, char **dst);
int				wordsize(char *line);
int				get_nextword(char *line);
int				get_firstword(char *line);
int				get_wordnumber(char *line);
int				expandhome(char **line, char *homepath, t_envs *env);

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

int				addenvequal(char *variable, t_envs *env);
int				addenvvar(char *name, char *value, t_envs *env);
int				init(int argc, char **argv, char **envp, t_envs *env);
int				copyenv(char **envp, t_envs *env);
int				setoptions(int argc, char **argv, t_envs *env);
int				init_empty_env(t_envs *env);

/*
** Utility functions
*/
char			*capitalize(char *str);
int				iswhitespace(char c);
int				isquote(char c);
int				incrementshlevel(t_envs *env);
int				pwdexist(t_var *list);
int				envvarexist(char *str, t_var *list);
int				returnval(int val, t_envs *env);
int				printvarvalue(char *str, t_var *list);
char			*getvarvalue(char *str, t_var *list);

/*
** Env function
*/

int				printenv(t_envs *env);
int				envshell(char **split, t_envs *env);
int				setenvshell(char **split, t_envs *env);
int				unsetenvshell(char **split, t_envs *env);

/*
** Echo function
*/

int				echo(char **instructions, t_envs *env);
int				echostr(char *str, t_envs *env);
int				echowithvariable(char *str, t_envs *env);
int				printvariable(char *str, int len, t_envs *env);
int				echostr(char *str, t_envs *env);

/*
** builtins
*/

int				pwdshell(char **splittedline, t_envs *env);
int				setoptions(int argc, char **argv, t_envs *env);
int				cdshell(char **splittedline, t_envs *env);
int				execprogram(char **splittedline, t_envs *env);
int				cdend(char *pathtofree, char *realpath, t_envs *env);
int				gobackpath(char *path);
int				isvalidpath(char *path);
int				swapoldpwd(t_envs *env);
int				isabsolute(char *str);
int				cdabsolute(char *path, t_envs *env);
int				exec(char *path, char **argv, t_envs *env);
int				freeenv(char **env);

#endif
