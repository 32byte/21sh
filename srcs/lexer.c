/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lportay <lportay@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/20 09:42:08 by lportay           #+#    #+#             */
/*   Updated: 2018/01/02 10:31:55 by lportay          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_21sh.h"


void	print_toklist(t_token *toklist)
{
	while (toklist)
	{
		ft_putstr("First Letter: \'");
		write(STDOUT_FILENO, (char *)toklist->first_letter->content, 1);
		ft_putstr("\' Last Letter: \'");
		write(STDOUT_FILENO, (char *)toklist->last_letter->content, 1);
		ft_putstr("\' Type: ");
		ft_putnbr(toklist->type);
		write(STDOUT_FILENO, "\n", 1);
		
		toklist = toklist->next;
	}
}


bool	isquote(char c)
{
	if (c == '\'' || c == '\\' || c == '"')
		return (true);
	else
		return (false);
}

bool 	ft_isblank(char c)
{
	if (c == ' ' || c == '\t')
		return (true);
	else
		return (false);
}

bool	is_simple_operator(char c)
{
	if (c == '|' || c == '&' || c == ';' || c == '<' || c == '>')
		return (true);
	else
		return (false);
}
bool is_extendable_operator(int type)
{
	if (type == OR || type == AND || type == LESS || type == GREAT)
		return (true);
	else if (type == LESSAND || type == GREATAND)
		return (true);	
	else if (type == DUP_INPUT || type == DUP_OUTPUT || type == IO_NUMBER_LESS || type == IO_NUMBER_GREAT)
		return (true);
	else
		return (false);
}

// returns true if end of token reached

bool	extend_word(t_token *token, char c, int quote_state)
{
	if ((ft_isblank(c) == true || is_simple_operator(c)) && quote_state == UNQUOTED)
		return (true);
	else if (token->type == WORD && c == '=')
		token->type = ASSIGNMENT_WORD;
	else if (token->type == NUMBER && ft_isdigit(c) == false) 
	{
		DEBUG;
		token->type = WORD;
	}
	return (false);	
}
		
// Returns true if the operator has been extended

bool	extend_operator(t_token *token, char c)
{
	if (ft_isblank(c) == true)
		return (false);
	else if (token->type == OR && c == '|')
		token->type = OR_IF;
	else if (token->type == AND && c == '&')
		token->type = AND_IF;
	else if (token->type == LESS && c == '<')
		token->type = DLESS;
	else if (token->type == LESS && c == '&')
		token->type = LESSAND;
	else if (token->type == GREAT && c == '>')
		token->type = DGREAT;
	else if (token->type == GREAT && c == '&')
		token->type = GREATAND;
	else if (token->type == NUMBER && c == '>')
		token->type = IO_NUMBER_GREAT;
	else if (token->type == NUMBER && c == '<')
		token->type = IO_NUMBER_LESS;
	else if (token->type == IO_NUMBER_LESS && (ft_isalpha(c) == true))
		token->type = FILE_TO_FD;
	else if (token->type == IO_NUMBER_GREAT && (ft_isalpha(c) == true))
		token->type = FD_TO_FILE;
	else if (token->type == IO_NUMBER_LESS && c == '&')
		token->type = DUP_INPUT;
	else if (token->type == IO_NUMBER_GREAT && c == '&')
		token->type = DUP_OUTPUT;
	else if (token->type == DUP_OUTPUT && c == '-')
		token->type = CLOSE_OUTPUT_FD;
	else if (token->type == DUP_INPUT && c == '-')
		token->type = CLOSE_INPUT_FD;
	else if (token->type == LESSAND && c == '-')
		token->type = CLOSE_STDOUT;
	else if (token->type == GREATAND && c == '-')
		token->type = CLOSE_STDERR;
	else
		return (false);
	return (true);
}


int		basic_token_type(char c, int quote_state)
{
	if (c == '\n')
		return (NEWLINE);
	else if (c == ';' && quote_state == UNQUOTED)
		return (SEMICOL);
	else if (c == '|' && quote_state == UNQUOTED)
		return (OR);
	else if (c == '&' && quote_state == UNQUOTED)
		return (AND);
	else if (c == '<' && quote_state == UNQUOTED)
		return (LESS);
	else if (c == '>' && quote_state == UNQUOTED)
		return (GREAT);
	else if (ft_isdigit(c) == true)
		return (NUMBER);
	else
		return (WORD);
}

t_token		*new_token(t_dlist *line)
{
	t_token *new;

	if (!(new = (t_token *)malloc(sizeof(t_token))))
		return (NULL);
	new->first_letter = line;
	new->last_letter = NULL;
	new->next = NULL;
	return (new);
}


void		apply_tokrules(t_token *last_tok, t_dlist *line, t_stack **quote)
{
	if (!line->next)
	{
		if (!last_tok->last_letter)
			last_tok->last_letter = line->previous;
		last_tok->next = new_token(line);
		last_tok->next->type = NEWLINE;
		last_tok->next->last_letter = line; 
		return ;
	}

//	if (isquote(*(char *)line->content) == true)
//		switch_state(quote, *(char *)line->content);


	if (last_tok->type == WORD || last_tok->type == NUMBER || last_tok->type == ASSIGNMENT_WORD)
		if (extend_word(last_tok, *(char *)line->content, (*quote)->state) == true && !last_tok->last_letter)
			last_tok->last_letter = line->previous;


//	if ((*quote)->state == UNQUOTED && is_extendable_operator(last_tok->type) == true)
//	{
//		if (extend_operator(last_tok, *(char *)line->content) == true)
//			last_tok->last_letter = line;
//		else
//			last_tok->last_letter = line->previous;
//	}
		
	if (last_tok->last_letter && ft_isblank(*(char *)line->content) == false && (*quote)->state == UNQUOTED)
	{
		last_tok->next = new_token(line);
		last_tok->next->type = basic_token_type(*(char *)line->content, (*quote)->state);

	//	if (last_tok->next->type == NEWLINE || last_tok->next->type == SEMICOL)
	//		last_tok->next->last_letter = line;
	}
	
//	if ((*quote)->state == BSLASH)
//		stack_pop(quote);
}

void	delete_toklist(t_token **toklist)
{
	t_token *tmp;

	tmp = *toklist;
	while (tmp)
	{
		*toklist = (*toklist)->next;
		free(tmp);
		tmp = *toklist;
	}
	*toklist = NULL;
}

t_token		*tokenizer(t_dlist *line)
{
	t_token *toklist;
	t_token *last_tok;
	t_stack *quote_state;

	quote_state = NULL;
	stack_push(&quote_state, stack_create(UNQUOTED));

	toklist = new_token(line);
	toklist->last_letter = line;
	toklist->type = HEAD;

	last_tok = toklist;
	
	line = line->next;
	while (line)
	{
		apply_tokrules(last_tok, line, &quote_state);
		line = line->next;
		if (last_tok->next)
			last_tok = last_tok->next;
	}
	print_toklist(toklist->next);
	stack_del(&quote_state);
	return (toklist);
}