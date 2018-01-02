/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_cursor.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lportay <lportay@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/19 16:52:48 by lportay           #+#    #+#             */
/*   Updated: 2017/12/19 18:22:05 by lportay          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_21sh.h"

void	move_cursor_n_columns(int n)
{
	char *tmp;

	if (!n)
		return ;
	write(STDIN_FILENO, "\E[", 2);
	tmp = ft_itoa(abs(n));
	ft_putstr_fd(STDIN_FILENO, tmp);
	if (n > 0)
		write(STDIN_FILENO, "C", 1);
	else if (n < 0)
		write(STDIN_FILENO, "D", 1);
	free(tmp);
}

void	move_cursor_n_lines(int n)
{
	char *tmp;

	if (!n)
		return ;
	write(STDIN_FILENO, "\E[", 2);
	tmp = ft_itoa(abs(n));
	ft_putstr_fd(STDIN_FILENO, tmp);
	if (n > 0)
		write(STDIN_FILENO, "B", 1);
	else if (n < 0)
		write(STDIN_FILENO, "A", 1);
	free(tmp);
}

void	move_cursor_forward(t_21sh *env)
{
	env->cursor_offset++;
	if (!(env->cursor_offset % env->ws.ws_col))
		tputs(env->tc.dow, 1, &ft_putchar_stdin);
	else
		tputs(env->tc.nd, 1, &ft_putchar_stdin);
}

void	move_cursor_backward(t_21sh *env)
{
	if (!(env->cursor_offset % env->ws.ws_col))
	{
		tputs(env->tc.up, 1, &ft_putchar_stdin);
		move_cursor_n_columns(env->ws.ws_col);		
	}
	else
		tputs(env->tc.le, 1, &ft_putchar_stdin);
	env->cursor_offset--;
}

void	move_cursor_end_of_line(t_21sh *env)
{
	move_cursor_n_lines(env->num_lines - env->cursor_line);
	tputs(env->tc.cr, 1, &ft_putchar_stdin);
	move_cursor_n_columns(env->line_len % env->ws.ws_col);
}