/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_21sh.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lportay <lportay@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/31 10:32:03 by lportay           #+#    #+#             */
/*   Updated: 2018/02/06 19:35:33 by vbastion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef FT_21SH_H
# define FT_21SH_H

# include "libft.h"

# include <sys/ioctl.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <termios.h>
# include <term.h>
# include <curses.h>
# include <fcntl.h>
# include <time.h>
# include <limits.h>
# include <pwd.h>
typedef struct s_21sh t_21sh;
# include "line.h"
# include "prompt.h"
# include "history.h"
# include "token.h"


# define NOENVIRON_STR			"Invalid Environment to use this program.\n"
# define NOMEM_STR				"No memory available for dynamic allocation\n"
# define NODIR_STR				"Error retrieving current directory\n"
# define FAILREAD_STR			"Can't read from STDIN\n"
# define BADQUOTES_STR			"Unexpected end of file\n"
# define FAILSETSIGHDLR_STR		"Couldn't set properly signal handlers.\n"

# define AMBIG_REDIR			"ambiguous redirect\n"

# define PATH					"/usr/local/bin/:/usr/bin:/bin"

//TO DELETE
# define DEBUG 		write(STDOUT_FILENO, "DEBUG\n", 6)
# define DEBUG1 	write(STDOUT_FILENO, "DEBUG1\n", 7)
# define DEBUG2 	write(STDOUT_FILENO, "DEBUG2\n", 7)
# define ERROR 		write(STDOUT_FILENO, "ERROR\n", 6)


enum				e_errcode
{
	SUCCESS,
	NOENVIRON,
	FAILREAD,
	NODIR,
	NOMEM,
	BADQUOTES,
	FAILSETSIGHDLR,
};

/*
** sizeof struct termios = 60
*/

//padder correctement la struct

truct					s_ctxt
{
	/*
	**	JOB CONTROL - Might miss a lot of variables
	*/
	
	pid_t				pid;
	pid_t				pgid;
	int					fd;
	char				**path;
	t_list				*running_processes;
	
	/*
	**	EXECUTION HASH (for HASH builtin)
	*/
	
	t_hash_dict			*hash;
	
	/*
	**	SHELL VARIABLES
	*/
	
	char				**env;
	char				**locals;
	
	/*
	**	LINE EDITION
	*/
	
	t_line				line;
	t_line				*cur_line;//line currently modified
	t_hist				hist;
	char                *heredoc_eof;//current EOF
	char				prompt_mode[4];
	
	/*
	**	SHOPT BOOLS
	*/
	
	int					emacs_mode;
	int	                line_edition;
	int	                history;
	
	/*
	**	TERMINAL CONTEXT
	*/
	
	struct s_termcaps	tc;
	struct winsize		ws;
	int	                istty;
	struct termios		tios;
	struct termios		oldtios;
	
	/*
	**	TBD
	*/
	
	t_token				*toklist; // Do we keep it here?
};

struct			s_21sh
{

	t_line				line;
	t_hist				hist;
	t_line				*cur_line;//line currently modified

	struct s_termcaps	tc;
	struct winsize		ws;
	struct termios		tios;
	struct termios		oldtios;

	t_kvp				*environ;
	t_kvp				*local;
	
	t_token				*toklist;


	char				*heredoc_eof;//current EOF
	char 				prompt_mode[4];
	bool				emacs_mode;
	bool				line_edition;
	bool				history;
	bool				istty;
};


/*
typedef struct			s_typefunc
{
	int type;
	void	(*f)(void *ptr);
}
*/

void	vingtetunsh(char **av, char **env);

void	dump_err(char errcode);
void	fatal_err(char errcode, t_21sh *env);
void	wrap_exit(int status, t_21sh *env);

int		wrap_signal(void);
void	sig_switch(int signum, t_21sh *env);
t_21sh *get_envaddr(t_21sh *envaddr);

/*
** Tools
*/

int		ft_putchar_stdin(int c);
bool	str_isblank(char *str);
bool	dlst_isblank(t_dlist *dlst);
bool	is_number(char *str);
bool	is_number_w_dash(char *str);
bool	is_redir(int type);

// Shell script stuff

//enum	e_rword
//{
//		BANG,
//		LBRACE,
//		RBRACE,
//		CASE,
//		DO,
//		DONE,
//		ELIF,
//		ELSE,
//		ESAC,
//		FI,
//		FOR,
//		IF,
//		IN,
//		THEN,
//		UNTIL,
//		WHILE,
//};

#endif
