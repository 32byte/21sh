/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lportay <lportay@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/08 20:11:48 by lportay           #+#    #+#             */
/*   Updated: 2018/04/10 15:40:01 by vbastion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_42sh.h"

/*
** Définit les comportements pour chaque signal
** SIGINT	==> Réaffiche le prompt ou kill les process en cours
** SIGWINCH ==> Window size change
** SIGTERM ==> Block it.
** SIGQUIT ==> Block it.
*/

static void	del_lines(t_line *l)
{
	if (l->line && l->line != l->lastline)
	{
		ft_dlstdel(&l->line, &delvoid);
		ft_dlstdel(&l->lastline, &delvoid);
	}
	else
	{
		l->line = NULL;
		ft_dlstdel(&l->lastline, &delvoid);
	}
}

static void	reset_line(t_ctx *ctx, t_line *l)
{
	if (ctx->line_edition)
	{
		write(STDOUT_FILENO, "^C", 2);
		go_end(ctx, l);
	}
	if (ctx->line_edition || (!ctx->line_edition && !l->eohdoc))
		write(STDOUT_FILENO, "\n", 1);
	if (l->split_line)
		ft_dlstdel(&l->split_line, &delvoid);
	if (l->line)
		ft_dlsthead(&l->line);
	del_lines(l);
	stack_del(&l->linestate);
	if (stack_create_push(&l->linestate, UNQUOTED) == -1)
		fatal_err(NOMEM, ctx);
	ft_strcpy(ctx->prompt_mode, PS1);
	if (l->eohdoc)
		ioctl(STDIN_FILENO, TIOCSTI, "\n");
	else
		init_line(l);
}

void		sighandler(int signum)
{
	t_ctx *ctx;

	ctx = get_ctxaddr();
	if (signum == SIGWINCH && ctx->line_edition)
	{
		ioctl(STDIN_FILENO, TIOCGWINSZ, &ctx->ws);
		if (ctx->cur_line)
		{
			clear_line(ctx, ctx->cur_line);
			redraw_line(ctx, ctx->cur_line);
		}
	}
}

void		sighand_int(int signo)
{
	t_ctx	*ctx;

	(void)signo;
	ctx = get_ctxaddr();
	reset_line(ctx, ctx->cur_line);
}

void		set_sighandler(void)
{
	signal(SIGWINCH, &sighandler);
	signal(SIGTSTP, SIG_IGN);
	signal(SIGINT, SIG_IGN);
	signal(SIGTERM, SIG_IGN);
	signal(SIGTTIN, SIG_IGN);
	signal(SIGTTOU, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}
