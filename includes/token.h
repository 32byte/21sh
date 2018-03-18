/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbastion <vbastion@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/25 12:30:05 by lportay           #+#    #+#             */
/*   Updated: 2018/03/18 15:24:13 by lportay          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_H
# define TOKEN_H

# define R_LESS (LESS | DLESS | TLESS | LESSAND)
# define R_GREAT (GREAT | DGREAT | GREATAND)
# define R_AND (ANDGREAT | ANDDGREAT)
# define RDIR (R_LESS | R_GREAT | R_AND)

enum	e_toktype
{
	HEAD,

	NEWLINE = 1 << 0, 				// \n
	SEMICOL = 1 << 1,				// ;
	IO_NUMBER = 1 << 2,				// *[0-9]>|*[0-9]<
	WORD = 1 << 3,       			// Everything else...
// NAME,
	ASSIGNMENT_WORD = 1 << 4, 		// NAME=

	OR = 1 << 5,					// |
	AND = 1 << 6,					// &
	LESS = 1 << 7,					// <
	GREAT = 1 << 8,					// >
	OR_IF = 1 << 9,					// ||
	AND_IF = 1 << 10, 				// &&
	DLESS = 1 << 11,				// << (HEREDOC)
	DGREAT = 1 << 12,				// >>
	LESSAND = 1 << 13,				// <&
	GREATAND = 1 << 14,				// >&
	TLESS = 1 << 15,				// <<<
	ANDGREAT = 1 << 16,				// &>
	ANDDGREAT = 1 << 17,			// &>>

	COMMENT = 1 << 20,				// # *ANY_TOKEN NEWLINE
	TOKERR = 1 << 21
};

typedef struct		s_token
{
	t_dlist 		*first_letter;
	t_dlist 		*last_letter;
	struct s_token	*next;
	enum e_toktype	type;
}					t_token;

typedef struct		s_redir
{
	t_dlist 		*first_letter;
	t_dlist 		*last_letter;
	struct s_token	*next;
	enum e_toktype	type;

	int				lhs;
	char			*s_rhs;
	int				fd_rhs;
	bool			dash;
}					t_redir;

typedef struct		s_heredoc
{
	t_dlist 		*first_letter;
	t_dlist 		*last_letter;
	struct s_token	*next;
	enum e_toktype	type;

	int				lhs;
	char			*s_rhs;
	int				glob;
	t_line			hdoc;
}					t_heredoc;

char				*str_from_token(t_token *tok);

void				init_token_table(t_kvp *tok);
void				dump_token(t_token *tok);
void    			print_toklist(t_token *toklist);
t_token				*new_token(t_dlist *line);

void				clear_following_redirs(t_token *toklist);
void				delete_toklist(t_token **toklist);

void				tokrules(t_token *last_tok, t_dlist *line, t_stack **quote);

t_token				*tokenizer(t_dlist *line);

t_token				*filter_tokens(t_token *toklist);// static ?

int					is_metachar(char c);

#endif
