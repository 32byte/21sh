/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filter_tokens.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lportay <lportay@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/22 13:27:54 by lportay           #+#    #+#             */
/*   Updated: 2018/03/18 15:15:17 by lportay          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_21sh.h"

/*
** Replace ASSIGNMENT_WORD by WORD when character in the part before the '='
** character does not respect the NAME Posix rule
*/

static void	filter_assignment_word(t_token *toklist)
{
	t_dlist *tmp;

	tmp = toklist->first_letter;
	if (ft_isdigit(*(char *)tmp->data) == true || *(char *)tmp->data == '=')
			toklist->type = WORD;
	while (*(char *)tmp->data != '=' && (toklist->type & ASSIGNMENT_WORD))
	{
		if (ft_isalnum(*(char *)tmp->data) == false && *(char *)tmp->data != '_')
			toklist->type = WORD;
		tmp = tmp->next;
	}
}

static int	filter_heredoc(t_token *toklist, t_token *prev)
{
	t_heredoc *tmp;

	if (!(toklist->next->type & (WORD | ASSIGNMENT_WORD)) || !(tmp = (t_heredoc *)malloc(sizeof(t_heredoc))))
		return (-1);
	prev->next = (t_token *)tmp;
	tmp->first_letter = toklist->first_letter;
	tmp->last_letter = toklist->next->last_letter;
	tmp->next = toklist->next->next;
	tmp->type = toklist->type;
	tmp->lhs = STDIN_FILENO;
	tmp->s_rhs = str_from_token(toklist->next);
	free(toklist->next);
	free(toklist);
	return (SUCCESS);
}

static int	filter_redir(t_token *toklist, t_token *prev)
{
	t_redir *tmp;

	if (toklist->type == DLESS)
		return (filter_heredoc(toklist, prev));
	if (!(toklist->next->type & (WORD | ASSIGNMENT_WORD /*| DOLLAR*/)) || !(tmp = (t_redir *)malloc(sizeof(t_redir))))
		return (-1);
	prev->next = (t_token *)tmp;
	tmp->first_letter = toklist->first_letter;
	tmp->last_letter = toklist->next->last_letter;
	tmp->next = toklist->next->next;
	tmp->type = toklist->type;
	if (tmp->type & R_LESS)
		tmp->lhs = STDIN_FILENO;
	else if (tmp->type & R_GREAT)
		tmp->lhs = STDOUT_FILENO;
	tmp->s_rhs = str_from_token(toklist->next);
	tmp->fd_rhs = -1;
	tmp->dash = false;
//	get_right_op(tmp);
//	printf("left:%d|fd:%d|s:%s|dash:%d\n", tmp->lhs, tmp->fd_rhs, tmp->s_rhs, tmp->dash);
	free(toklist->next);
	free(toklist);
	return (SUCCESS);
}

/*
** IO_NUMBER filter
*/

static int	filter_io_number(t_token *toklist, t_token *prev)
{
	t_redir *tmp;
	char	*s;

	if (!(toklist->next->next->type & (WORD | ASSIGNMENT_WORD)) || !(tmp = (t_redir *)malloc(sizeof(t_redir))))
		return (-1);
	prev->next = (t_token *)tmp;
	tmp->first_letter = toklist->first_letter;
	tmp->last_letter = toklist->next->next->last_letter;
	tmp->next = toklist->next->next->next;
	tmp->type = toklist->next->type;
	s = str_from_token(toklist);
	tmp->lhs = ft_atoi(s);
	free(s);
	tmp->s_rhs = str_from_token(toklist->next->next);
	tmp->fd_rhs = -1;
	tmp->dash = false;
//	get_right_op(tmp);
//	printf("left:%d|fd:%d|s:%s|dash:%d\n", tmp->lhs, tmp->fd_rhs, tmp->s_rhs, tmp->dash);
	free(toklist->next->next);
	free(toklist->next);
	free(toklist);
	return (SUCCESS);
}

/*
** Change the following tokens:

** ASSIGNMENT_WORD

** IO_NUMBER

** LESS		
** GREAT	
** DLESS	
** DGREAT	
** LESSAND	
** GREATAND	
** TLESS	
** ANDGREAT	
** ANDDGREAT

** DOLLAR	

** BANG		
** 
** Into:
**
** Complete redirections operations with both operands and the operator in a single token |OK|
** PARAM_EXP
** CMD_SUB  
** ARI_EXP  
**
** Return the first faulty redirection if any.
*/

t_token	*filter_tokens(t_token *toklist)
{
	t_token *prev;

	prev = toklist;
	toklist = toklist->next;
	while (toklist->type != NEWLINE)
	{
		if (toklist->type & ASSIGNMENT_WORD)
			filter_assignment_word(toklist);
		else if (toklist->type & IO_NUMBER)
		{
			if (filter_io_number(toklist, prev) == -1)
				return (toklist->next);
		}
		else if (toklist->type & RDIR)
		{
			if (filter_redir(toklist, prev) == -1)
				return (toklist);
		}

		prev = prev->next;
		toklist = prev->next;
	}
	return (NULL);
}
