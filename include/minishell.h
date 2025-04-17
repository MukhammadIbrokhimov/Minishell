/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muxammad <muxammad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 19:48:31 by muxammad          #+#    #+#             */
/*   Updated: 2025/04/17 10:53:05 by muxammad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _MINISHELL_

# define _MINISHELL_

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <string.h>
# include <errno.h>
# include <sys/wait.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/stat.h>
# include <dirent.h>
# include <termios.h>
# include "libft/libft.h"

# define EXEC  1
# define REDIR 2
# define PIPE  3
# define LIST  4
# define BACK  5

# define MAXARGS 10

typedef struct s_env {
    char *name;
    char *value;
    struct s_env *next;
} t_env;

typedef struct s_shell {
    t_env *env_list;
    int exit_status;
    int in_heredoc;
} t_shell;

#endif