/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbastion <vbastion@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/12 19:10:15 by lportay           #+#    #+#             */
/*   Updated: 2018/02/12 14:08:37 by vbastion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_21sh.h"

/*
** Function which print the right error message
*/

void	dump_err(char errcode)
{
	if (errcode == BADQUOTES)
		ft_putstr_fd(BADQUOTES_STR, STDERR_FILENO);
	if (errcode == NOMEM)
		ft_putstr_fd(NOMEM_STR, STDERR_FILENO);
	if (errcode == NODIR)
		ft_putstr_fd(NODIR_STR, STDERR_FILENO);
	if (errcode == FAILSETSIGHDLR)
		ft_putstr_fd(FAILSETSIGHDLR_STR, STDERR_FILENO);
	if (errcode == FAILREAD)
		ft_putstr_fd(FAILREAD_STR, STDERR_FILENO);
}

/*
** Print the error message and exit
*/

void		fatal_err(char errcode, t_ctx *ctx)
{
	dump_err(errcode);
	wrap_exit(EXIT_FAILURE, ctx);
}

/*
** Free everything
*/

void	wrap_exit(int status, t_ctx *ctx)
{
	if (ctx->line.line)
	//ft_dlsthead(&ctx->line);//
		ft_dlstdel(&ctx->line.line, &delvoid);
	if (ctx->line.yank)
	//ft_dlsthead(&ctx->yank);//
		ft_dlstdel(&ctx->line.yank, &delvoid);

//	if (ctx->line.linestate)
//		stack_del(&ctx->line.linestate);

	if (ctx->hist.list)//faire une fonction dédiée
	{
	////ft_dlsthead(&ctx->histlist);//
		if (ctx->hist.list->next)
		{
			trim_history(&ctx->hist.list->next, ft_astr_getval(ctx->locals, "HISTFILESIZE"));
			save_history(ctx->locals, ctx->hist.list->next);
			ctx->hist.list = ctx->hist.list->next;
			ft_dlstdelone(&ctx->hist.list->prev, &delvoid);
			ft_dlstdel(&ctx->hist.list, &del_histentry);
		}
		else
			ft_dlstdelone(&ctx->hist.list, &delvoid);
	}
	if (ctx->environ)
		ft_astr_clear(&ctx->environ);
	if (ctx->locals)
		ft_astr_clear(&ctx->locals);
	write(STDOUT_FILENO, "exit\n", 5);
	if (ctx->line_edition == true)
		tcsetattr(STDIN_FILENO, TCSADRAIN, &ctx->oldtios);	
	exit(status);
}
