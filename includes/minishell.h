/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toliver <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/12 01:07:10 by toliver           #+#    #+#             */
/*   Updated: 2020/01/12 19:09:20 by toliver          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include "libftprintf.h" 

# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/uio.h>
# include <dirent.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <signal.h>
# include <limits.h>

/*
** FONCTIONS AUTORISEES
** malloc, free, access, open, close, read, write, opendir, readdir, closedir,
** getcwd, chdir, stat, lstat, fstat, fork, execve, wait, waitpid, wait3, wait4
** signal, kill, exit
*/

enum					e_crash
{
	MALLOC_FAIL,
};

enum					e_error_origin
{
	ENV,
	SETENV,
	UNSETENV,
	EXPAND,
};

enum					e_warning_type
{
	MISSING_VARIABLE,
	MISSING_EQUAL,
	MISSING_HOME,
	MISSING_CWD,
};

typedef struct			s_envp
{
	char				**env;
	int					size;
	int					occupied;
}						t_envp;

typedef struct			t_env
{
	char				*prog_name;
	t_envp				envp;
	char				**commands;
	char				**command;
}						t_env;

/*
** INIT FUNCTIONS
*/

int					ft_env_init(t_env *env, char **av, char **envp);
void				ft_free_env(t_env *env);
void				ft_free_command(char **command);

/*
** ENV HANDLING FUNCTIONS
*/

int					ft_envp_set_(t_env *env);
int					ft_envp_realloc(t_env *env, int size);
int					ft_env(t_env *env);
int					ft_setenv(t_env *env, char *value);
int					ft_unsetenv(t_env *env, char *value);
int					ft_addenv(t_env *env, char *value);
char*				ft_env_get_value(t_env *env, char *value);
int					ft_env_get_index(t_env *env, char *value);

/*
** UTILS
*/

int					ft_is_charset(char c, char *charset);
int					ft_error(int irogin, int value, char *param, t_env *env);
int					ft_crash(int value, char *param, t_env *env);
int					ft_warning(int origin, int value, char *param, t_env *env);
int					ft_usage(void);
char				**ft_split_charset(char *str, char *charset);

#endif
